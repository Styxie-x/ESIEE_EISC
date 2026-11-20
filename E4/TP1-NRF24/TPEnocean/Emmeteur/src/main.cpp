#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <DFRobot_ENS160.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define serial_baud_rate 115200

//****** CE and CSN are configurable, specified values for Arduino NANO as connected above
#define CE_PIN 7
#define CSN_PIN 8
char c;
char serialCommand[16];
uint8_t serialCommandLength = 0;
const uint8_t enoceanButtonPattern[] = {0x00, 0x3C, 0x2F, 0x39};
uint8_t enoceanPatternIndex = 0;
unsigned long lastEnoceanButtonMillis = 0;
const unsigned long enoceanButtonDebounce = 300;

RF24 radio(CE_PIN, CSN_PIN);

//byte addresse[6] = "1Node";
const uint64_t addresses[2] = {0xa1a1a1a1a1, 0xd1d1d1d1d1};

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1 to distinguish the two nodes  ***/
bool radioNumber = 1;

// Used to control whether this node is sending or receiving
int Led = 2;

// Capteurs environnementaux
Adafruit_AHTX0 aht;
DFRobot_ENS160_I2C ens160(&Wire, 0x53);

// Timing non-bloquant
unsigned long previousMillis = 0;
const unsigned long interval = 2000; // Intervalle de lecture et transmission : 2 secondes

// Variables de stockage des mesures
float currentTemp = 0.0;
float currentHumidity = 0.0;
uint16_t currentECO2 = 0;
uint16_t currentTVOC = 0;
uint8_t currentAQI = 0;

// --- Fonctions capteurs ---

void initSensors() {
    Wire.begin();

    if (!aht.begin()) {
        Serial.println(F("[ERR] AHT introuvable."));
    }

    if (ens160.begin() != NO_ERR) {
        Serial.println(F("[ERR] ENS160 introuvable."));
    } else {
        ens160.setPWRMode(ENS160_STANDARD_MODE);
    }
}

void readAHT(float &temp, float &hum) {
    sensors_event_t h_event, t_event;
    aht.getEvent(&h_event, &t_event);
    temp = t_event.temperature;
    hum = h_event.relative_humidity;
}

void updateAirQuality(float temp, float hum, uint16_t &eco2, uint16_t &tvoc, uint8_t &aqi) {
    // Compensation dynamique en température et humidité
    ens160.setTempAndHum(temp, hum);

    eco2 = ens160.getECO2();
    tvoc = ens160.getTVOC();
    aqi  = ens160.getAQI();
}

void sendSCPITemperature(float temp) {
    char payload[32];
    // SCPI measurement format: :MEAS:TEMP <val>\n
    // dtostrf formats the float on AVR architecture without bloating printf
    char tempStr[8];
    dtostrf(temp, 4, 2, tempStr);
    snprintf(payload, sizeof(payload), ":MEAS:TEMP %s\n", tempStr);

    radio.stopListening();
    bool success = radio.write(payload, strlen(payload));
    radio.startListening();

    Serial.print(F("[TX SCPI] "));
    Serial.print(payload); // payload already includes '\n'
    if (!success) {
        Serial.println(F(" -> [TX ERR] NACK / Target unreachable"));
    }
}

void sendRadioPayload(const char *payload, uint8_t length) {
    radio.stopListening();
    bool success = radio.write(payload, length);
    radio.startListening();

    if (!success) {
        Serial.println(F(" -> [TX ERR] NACK / Target unreachable"));
    }
}

void handleEnoceanByte(uint8_t value) {
    if (value == enoceanButtonPattern[enoceanPatternIndex]) {
        ++enoceanPatternIndex;
        if (enoceanPatternIndex == sizeof(enoceanButtonPattern)) {
            enoceanPatternIndex = 0;
            unsigned long now = millis();
            if (now - lastEnoceanButtonMillis >= enoceanButtonDebounce) {
                const char toggleCommand = 'B';
                sendRadioPayload(&toggleCommand, sizeof(toggleCommand));
                lastEnoceanButtonMillis = now;
                serialCommandLength = 0;
                Serial.println(F("[ENOCEAN] Bouton detecte -> [NRF] B"));
            }
        }
    } else {
        enoceanPatternIndex = value == enoceanButtonPattern[0] ? 1 : 0;
    }
}

void processSerialCommand() {
    serialCommand[serialCommandLength] = '\0';

    if (serialCommandLength >= 3 && serialCommand[0] == 'S' && serialCommand[1] == ':') {
        int angle = 0;
        for (uint8_t index = 2; index < serialCommandLength; index++) {
            if (serialCommand[index] < '0' || serialCommand[index] > '9') {
                Serial.println(F("[ERR] Format attendu : S:angle (0..180)"));
                serialCommandLength = 0;
                return;
            }
            angle = angle * 10 + (serialCommand[index] - '0');
        }

        if (angle > 180) {
            Serial.println(F("[ERR] Angle servo hors limites (0..180)"));
            serialCommandLength = 0;
            return;
        }

        char payload[8];
        uint8_t payloadLength = snprintf(payload, sizeof(payload), "S:%d", angle);
        sendRadioPayload(payload, payloadLength);
        Serial.print(F("[TX SERVO] "));
        Serial.println(payload);
    } else if (serialCommandLength == 1) {
        c = serialCommand[0];
        sendRadioPayload(&c, sizeof(c));
        Serial.println(c);
    } else if (serialCommandLength > 0) {
        Serial.println(F("[ERR] Commande inconnue. Utiliser S:angle"));
    }

    serialCommandLength = 0;
}

void updateAndDisplaySensors() {
    readAHT(currentTemp, currentHumidity);
    updateAirQuality(currentTemp, currentHumidity, currentECO2, currentTVOC, currentAQI);

    Serial.print(F("[CAPTEURS] Temp : "));
    Serial.print(currentTemp, 1);
    Serial.print(F(" °C | Hum : "));
    Serial.print(currentHumidity, 1);
    Serial.print(F(" % | eCO2 : "));
    Serial.print(currentECO2);
    Serial.print(F(" ppm | TVOC : "));
    Serial.print(currentTVOC);
    Serial.print(F(" ppb | AQI : "));
    Serial.println(currentAQI);

    // Envoi SCPI sur nRF24
    sendSCPITemperature(currentTemp);
}

// --- Setup & Loop ---

void setup() {
    Serial.begin(serial_baud_rate);
    printf_begin();

    // Initialisation des capteurs I2C
    initSensors();

    radio.begin();
    radio.setRetries(5, 15);
    radio.setDataRate(RF24_2MBPS);
    radio.setChannel(30);
    radio.setPALevel(RF24_PA_HIGH);

    if (radioNumber) {
        radio.openWritingPipe(addresses[1]);
        radio.openReadingPipe(1, addresses[0]);
    } else {
        radio.openWritingPipe(addresses[0]);
        radio.openReadingPipe(1, addresses[1]);
    }

    radio.printDetails();
    Serial.println(F("\n*** PRESS 'T' to begin transmitting to the other node*****\n"));

    radio.startListening();
    pinMode(Led, OUTPUT);
}

void loop() {
    // 1. Gestion transmission radio sur saisie série
    while (Serial.available()) {
        uint8_t received = static_cast<uint8_t>(Serial.read());
        handleEnoceanByte(received);

        if (received > 0x7F || received == 0x00) {
            continue;
        }

        if (received == '\n' || received == '\r') {
            if (received == '\n') {
                processSerialCommand();
            }
        } else if (serialCommandLength < sizeof(serialCommand) - 1) {
            serialCommand[serialCommandLength++] = received;
        } else {
            serialCommandLength = 0;
            Serial.println(F("[ERR] Commande trop longue"));
        }
    }

    // 2. Gestion non-bloquante de l'affichage des capteurs et de l'envoi SCPI
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        updateAndDisplaySensors();
    }
}