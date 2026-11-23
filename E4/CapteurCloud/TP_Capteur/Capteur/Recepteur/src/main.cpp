#include <Arduino.h>
#include <RF24.h>

// Le canal, le débit, l’adresse et la taille de trame doivent correspondre à l’autre carte.
constexpr uint8_t RADIO_CE = 7;
constexpr uint8_t RADIO_CSN = 8;
constexpr uint8_t RADIO_CHANNEL = 30;
constexpr rf24_datarate_e RADIO_RATE = RF24_2MBPS;
constexpr uint64_t STATION_ADDRESS = 0xd1d1d1d1d1ULL;
constexpr uint8_t RADIO_PAYLOAD_SIZE = 32;

// Le format radio emploie les mêmes types et le même ordre des octets sur les deux Uno.
constexpr uint32_t MEASUREMENT_MAGIC = 0x314D4854UL; // Octets ASCII THM1
struct __attribute__((packed)) Measurement {
    uint32_t magic;
    uint32_t sequence;
    int16_t temperatureCentiC;
    uint16_t humidityCentiPct;
};

static_assert(sizeof(Measurement) == 12, "Format radio inattendu");

bool configureRadio(RF24 &radio) {
    if (!radio.begin()) {
        return false;
    }
    radio.setChannel(RADIO_CHANNEL);
    radio.setRetries(5, 15);
    radio.setAddressWidth(5);
    radio.setAutoAck(true);
    radio.setPayloadSize(RADIO_PAYLOAD_SIZE);
    return radio.getChannel() == RADIO_CHANNEL && radio.setDataRate(RADIO_RATE);
}

RF24 radio(RADIO_CE, RADIO_CSN);

void setup() {
    Serial.begin(115200);
    if (!configureRadio(radio)) {
        Serial.println(F("[RX] Echec initialisation nRF24 / debit incompatible"));
        while (true) {
            delay(1000);
        }
    }
    radio.setPALevel(RF24_PA_HIGH); // Puissance ACK fixe pendant les essais
    radio.openReadingPipe(1, STATION_ADDRESS);
    radio.startListening();

    Serial.println(F("sequence,temperature_c,humidite_pct"));
}

void loop() {
    if (!radio.available()) {
        return;
    }

    Measurement measurement;
    radio.read(&measurement, sizeof(measurement));
    if (measurement.magic != MEASUREMENT_MAGIC) {
        return;
    }

    if (measurement.temperatureCentiC < -4000 ||
        measurement.temperatureCentiC > 8000 ||
        measurement.humidityCentiPct > 10000) {
        Serial.println(F("[RX] Mesure hors plage"));
        return;
    }

    Serial.print(measurement.sequence);
    Serial.print(',');
    Serial.print(measurement.temperatureCentiC / 100.0f, 2);
    Serial.print(',');
    Serial.println(measurement.humidityCentiPct / 100.0f, 2);
}