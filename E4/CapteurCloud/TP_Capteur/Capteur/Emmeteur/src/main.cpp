#include <Arduino.h>
#include <RF24.h>
#include <DHT.h>

// Le canal, le débit, l’adresse et la taille de trame doivent correspondre à l’autre carte.
constexpr uint8_t RADIO_CE = 7;
constexpr uint8_t RADIO_CSN = 8;
//
constexpr uint8_t RADIO_CHANNEL = 30;
//
constexpr rf24_datarate_e RADIO_RATE = RF24_2MBPS;
constexpr uint64_t STATION_ADDRESS = 0xd1d1d1d1d1ULL;
constexpr uint8_t RADIO_PAYLOAD_SIZE = 32;
//
constexpr rf24_pa_dbm_e RADIO_TX_POWER = RF24_PA_MAX; // RF24_PA_MAX peut saturer le récepteur distant
constexpr unsigned long SAMPLE_INTERVAL_MS = 2;
static_assert(SAMPLE_INTERVAL_MS >= 2, "DHT22: intervalle minimal de 2 s");

enum class IdleMode : uint8_t { Listening, Standby, PowerDown };
constexpr IdleMode TX_IDLE_MODE = IdleMode::PowerDown;
constexpr bool TX_LOGS = false;

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

constexpr uint8_t DHT_PIN = 2;
constexpr uint8_t CYCLE_MARKER_PIN = 4;
constexpr uint8_t RADIO_MARKER_PIN = 5;
static_assert(sizeof(Measurement) <= RADIO_PAYLOAD_SIZE, "Trame trop longue");

DHT dht(DHT_PIN, DHT22);
RF24 radio(RADIO_CE, RADIO_CSN);
unsigned long lastSampleMs = 0;
uint32_t sequence = 0;
char serialCommand[16];
uint8_t serialCommandLength = 0;
bool serialCommandOverflow = false;

void enterIdleMode() {
    switch (TX_IDLE_MODE) {
        case IdleMode::Listening:
            radio.startListening();
            break;
        case IdleMode::Standby:
            break;
        case IdleMode::PowerDown:
            radio.powerDown();
            break;
    }
}

bool transmit(const void *payload, uint8_t length) {
    digitalWrite(RADIO_MARKER_PIN, HIGH);
    radio.powerUp();
    radio.stopListening();
    bool acknowledged = radio.write(payload, length);
    enterIdleMode();
    digitalWrite(RADIO_MARKER_PIN, LOW);
    return acknowledged;
}

void sampleAndTransmit() {
    digitalWrite(CYCLE_MARKER_PIN, HIGH);
    ++sequence;
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    bool valid = temperature >= -40.0f && temperature <= 80.0f &&
                 humidity >= 0.0f && humidity <= 100.0f;
    bool acknowledged = false;

    if (valid) {
        const Measurement measurement{
            MEASUREMENT_MAGIC, sequence,
            static_cast<int16_t>(lroundf(temperature * 100.0f)),
            static_cast<uint16_t>(lroundf(humidity * 100.0f))
        };
        acknowledged = transmit(&measurement, sizeof(measurement));
    }
    digitalWrite(CYCLE_MARKER_PIN, LOW);

    if (TX_LOGS) {
        Serial.print(F("[TX] #"));
        Serial.print(sequence);
        if (!valid) {
            Serial.println(F(" ERREUR DHT22"));
        } else {
            Serial.println(acknowledged ? F(" ACK") : F(" NACK"));
        }
    }
}

void handleSerial() {
    while (Serial.available()) {
        int received = Serial.read();
        if (received == '\r' || received == '\n') {
            if (!serialCommandOverflow && serialCommandLength == 1) {
                bool acknowledged = transmit(serialCommand, 1);
                Serial.println(acknowledged ? F("[TX] ACK") : F("[TX] NACK"));
            } else if (serialCommandLength || serialCommandOverflow) {
                Serial.println(F("[TX] Utiliser une commande d'un caractere"));
            }
            serialCommandLength = 0;
            serialCommandOverflow = false;
        } else if (received > 0 && received <= 0x7F) {
            if (serialCommandLength < sizeof(serialCommand)) {
                serialCommand[serialCommandLength++] = static_cast<char>(received);
            } else {
                serialCommandOverflow = true;
            }
        }
    }
}

void setup() {
    pinMode(CYCLE_MARKER_PIN, OUTPUT);
    pinMode(RADIO_MARKER_PIN, OUTPUT);
    Serial.begin(115200);
    dht.begin();
    if (!configureRadio(radio)) {
        Serial.println(F("[TX] Echec initialisation nRF24 / debit incompatible"));
        while (true) {
            delay(1000);
        }
    }
    radio.setPALevel(RADIO_TX_POWER);
    radio.openWritingPipe(STATION_ADDRESS);
    enterIdleMode();
    lastSampleMs = millis();
    Serial.println(F("[TX] Pret"));
}

void loop() {
    const unsigned long now = millis();
    if (now - lastSampleMs >= SAMPLE_INTERVAL_MS) {
        lastSampleMs = now;
        sampleAndTransmit();
    }
    handleSerial();
}
