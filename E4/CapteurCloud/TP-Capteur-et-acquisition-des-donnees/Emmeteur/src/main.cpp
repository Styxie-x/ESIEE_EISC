#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <DHT.h>
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

RF24 radio(CE_PIN, CSN_PIN);

//byte addresse[6] = "1Node";
const uint64_t addresses[2] = {0xa1a1a1a1a1, 0xd1d1d1d1d1};

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1 to distinguish the two nodes  ***/
bool radioNumber = 1;

// Capteur DHT22 sur la broche D2
#define DHT_PIN 2
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

// Timing non-bloquant
unsigned long previousMillis = 0;
const unsigned long interval = 2000; // Intervalle de lecture et transmission : 2 secondes

// Variables de stockage des mesures
float currentTemp = 0.0;
float currentHumidity = 0.0;

// --- Fonctions capteurs ---

void initSensors() {
    dht.begin();
}

bool readDHT22(float &temp, float &hum) {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        return false;
    }

    temp = temperature;
    hum = humidity;
    return true;
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

void sendSCPIHumidity(float humidity) {
    char payload[32];
    char humidityStr[8];
    dtostrf(humidity, 4, 2, humidityStr);
    snprintf(payload, sizeof(payload), ":MEAS:HUM %s\n", humidityStr);

    radio.stopListening();
    bool success = radio.write(payload, strlen(payload));
    radio.startListening();

    Serial.print(F("[TX SCPI] "));
    Serial.print(payload);
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

void processSerialCommand() {
    serialCommand[serialCommandLength] = '\0';

    if (serialCommandLength == 1) {
        c = serialCommand[0];
        sendRadioPayload(&c, sizeof(c));
        Serial.println(c);
    } else if (serialCommandLength > 0) {
        Serial.println(F("[ERR] Commande inconnue. Utiliser A, E, C ou T"));
    }

    serialCommandLength = 0;
}

void updateAndDisplaySensors() {
    if (!readDHT22(currentTemp, currentHumidity)) {
        Serial.println(F("[CAPTEURS] Erreur lecture DHT22"));
        return;
    }

    Serial.print(F("[CAPTEURS] Temp : "));
    Serial.print(currentTemp, 1);
    Serial.print(F(" °C | Hum : "));
    Serial.print(currentHumidity, 1);
    Serial.println(F(" %"));

    // Envoi SCPI sur nRF24
    sendSCPITemperature(currentTemp);
    sendSCPIHumidity(currentHumidity);
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
}

void loop() {
    // 1. Gestion transmission radio sur saisie série
    while (Serial.available()) {
        uint8_t received = static_cast<uint8_t>(Serial.read());

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