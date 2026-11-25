#!/usr/bin/env bash
set -eu

if [[ $# -ne 2 ]]; then
    printf 'Usage : bash enregistrer.sh PORT FICHIER.csv\n' >&2
    exit 2
fi

port=$1
fichier=$2
if [[ ! -c "$port" ]]; then
    printf 'Port série introuvable : %s\n' "$port" >&2
    exit 1
fi

# Garder le port ouvert pendant la configuration et la lecture.
exec 3<>"$port"
stty 115200 raw -echo -hupcl <&3

# Protéger une capture précédente du même nom.
set -o noclobber
exec 4>"$fichier"
printf 'Capture dans %s. Arrêt avec Ctrl+C.\n' "$fichier" >&2

LC_ALL=C awk '
    BEGIN {
        debut = systime()
        print "temps_s,sequence,temperature_c,humidite_pct"
        fflush()
    }
    {
        sub(/\r$/, "")
        if ($0 ~ /^[0-9]+,-?[0-9]+\.[0-9]+,[0-9]+\.[0-9]+$/) {
            ligne = (systime() - debut) "," $0
            print ligne
            print ligne > "/dev/stderr"
            fflush()
        }
    }
' <&3 >&4
