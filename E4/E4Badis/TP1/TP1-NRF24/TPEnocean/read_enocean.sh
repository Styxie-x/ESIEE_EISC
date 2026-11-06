#!/usr/bin/env bash

set -euo pipefail

DEVICE="${1:-/dev/ttyUSB2}"
ARDUINO_DEVICE="${2:-/dev/ttyUSB0}"
OLD_SETTINGS="$(stty -F "$DEVICE" -g)"
ARDUINO_OLD_SETTINGS="$(stty -F "$ARDUINO_DEVICE" -g)"

restore() {
    stty -F "$DEVICE" "$OLD_SETTINGS" 2>/dev/null || true
    stty -F "$ARDUINO_DEVICE" "$ARDUINO_OLD_SETTINGS" 2>/dev/null || true
}

trap restore EXIT
trap 'exit 130' INT TERM

stty -F "$DEVICE" 57600 raw -echo
stty -F "$ARDUINO_DEVICE" 115200 raw -echo

echo "Écoute de $DEVICE"
echo "Transfert vers $ARDUINO_DEVICE"
echo "Recherche : 00 3c 2f 39"
echo "Ctrl-C pour arrêter"

python3 -u - "$DEVICE" "$ARDUINO_DEVICE" <<'PY'
import os
import sys
from datetime import datetime

device = sys.argv[1]
arduino_device = sys.argv[2]
target = bytes.fromhex("00 3c 2f 39")
window = bytearray()
count = 0

fd = os.open(device, os.O_RDONLY)
arduino_fd = os.open(arduino_device, os.O_WRONLY)
try:
    while True:
        data = os.read(fd, 4096)
        if not data:
            break

        os.write(arduino_fd, data)
        window.extend(data)
        while True:
            position = window.find(target)
            if position < 0:
                del window[:-len(target) + 1]
                break

            count += 1
            now = datetime.now().strftime("%H:%M:%S")
            print(f"[{now}] ID détecté ({count}) : 00 3c 2f 39", flush=True)
            del window[:position + len(target)]
finally:
    os.close(fd)
    os.close(arduino_fd)
PY
