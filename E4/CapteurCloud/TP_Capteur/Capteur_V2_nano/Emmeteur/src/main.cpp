#include <Arduino.h>
#include <Wire.h>
#include <RF24.h>
#include <Adafruit_AHTX0.h>
#include <DFRobot_ENS160.h>

// Le canal, le débit, l’adresse et la taille de trame doivent correspondre à l’autre carte.
constexpr uint8_t RADIO_CE = 7;
constexpr uint8_t RADIO_CSN = 8;
//
constexpr uint8_t RADIO_CHANNEL = 30;
//
constexpr rf24_datarate_e RADIO_RATE = RF24_2MBPS; // RF24_2MBPS peut saturer le récepteur distant

constexpr uint64_t STATION_ADDRESS = 0xd1d1d1d1d1ULL;
constexpr uint8_t RADIO_PAYLOAD_SIZE = 32;
//
constexpr rf24_pa_dbm_e RADIO_TX_POWER = RF24_PA_HIGH; // RF24_PA_MAX peut saturer le récepteur distant
constexpr unsigned long SAMPLE_INTERVAL_MS = 2;

enum class IdleMode : uint8_t { Listening, Standby, PowerDown };
constexpr IdleMode TX_IDLE_MODE = IdleMode::PowerDown;
constexpr bool TX_LOGS = true;

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

constexpr uint8_t CYCLE_MARKER_PIN = 4;
constexpr uint8_t RADIO_MARKER_PIN = 5;
static_assert(sizeof(Measurement) <= RADIO_PAYLOAD_SIZE, "Trame trop longue");

Adafruit_AHTX0 aht;
DFRobot_ENS160_I2C ens160(&Wire, 0x53);
RF24 radio(RADIO_CE, RADIO_CSN);
bool ahtReady = false;
bool ensReady = false;
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

    sensors_event_t humidityEvent;
    sensors_event_t temperatureEvent;
    humidityEvent.relative_humidity = 0.0f;
    temperatureEvent.temperature = 0.0f;
    bool ahtValid = ahtReady && aht.getEvent(&humidityEvent, &temperatureEvent);
    float temperature = temperatureEvent.temperature;
    float humidity = humidityEvent.relative_humidity;

    bool valid = ahtValid &&
                 temperature >= -40.0f && temperature <= 80.0f &&
                 humidity >= 0.0f && humidity <= 100.0f;
    bool acknowledged = false;

    uint16_t eco2 = 0;
    uint16_t tvoc = 0;
    uint8_t aqi = 0;
    if (ensReady) {
        ens160.setTempAndHum(temperature, humidity);
        eco2 = ens160.getECO2();
        tvoc = ens160.getTVOC();
        aqi = ens160.getAQI();
    }

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
            Serial.println(F(" ERREUR AHT20"));
        } else {
            Serial.print(temperature, 1);
            Serial.print(F(" C | "));
            Serial.print(humidity, 1);
            Serial.print(F(" % | eCO2 "));
            Serial.print(eco2);
            Serial.print(F(" ppm | TVOC "));
            Serial.print(tvoc);
            Serial.print(F(" ppb | AQI "));
            Serial.print(aqi);
            Serial.println(acknowledged ? F(" | ACK") : F(" | NACK"));
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
    Wire.begin();
    ahtReady = aht.begin();
    ensReady = ens160.begin() == NO_ERR;
    if (ensReady) {
        ens160.setPWRMode(ENS160_STANDARD_MODE);
    }
    if (!ahtReady) {
        Serial.println(F("[TX] AHT20 introuvable"));
    }
    if (!ensReady) {
        Serial.println(F("[TX] ENS160 introuvable"));
    }
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
