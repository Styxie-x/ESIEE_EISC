#include <Arduino.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define serial_baud_rate 115200

#define CE_PIN 7
#define CSN_PIN 8

RF24 radio(CE_PIN, CSN_PIN);

const uint64_t addresses[2] = {0xa1a1a1a1a1, 0xd1d1d1d1d1};

// Configuration du nœud récepteur
bool radioNumber = 0; 
const uint8_t Led = 2;

// Machine à états pour la LED
enum LedMode {
    LED_OFF,
    LED_ON,
    LED_BLINK,
    LED_TEMP_MONITOR
};

LedMode currentMode = LED_OFF;

// Gestion du clignotement non bloquant
unsigned long previousBlinkMillis = 0;
const unsigned long blinkInterval = 250; // demi-période (ms)
bool blinkState = false;

// Buffer de réception radio
char rxBuffer[33];

void setLedMode(LedMode newMode) {
    currentMode = newMode;
    switch (currentMode) {
        case LED_ON:
            digitalWrite(Led, HIGH);
            Serial.println(F("[MODE] LED Allumee fixe (A)"));
            break;
        case LED_OFF:
            digitalWrite(Led, LOW);
            Serial.println(F("[MODE] LED Eteinte (E)"));
            break;
        case LED_BLINK:
            Serial.println(F("[MODE] Clignotement actif (C)"));
            break;
        case LED_TEMP_MONITOR:
            Serial.println(F("[MODE] Surveillance temperature active (T)"));
            break;
    }
}

void processCommand(const char* payload) {
    // 1. Commandes directes sur un caractère
    if (payload[0] == 'A' && (payload[1] == '\0' || payload[1] == '\r' || payload[1] == '\n')) {
        setLedMode(LED_ON);
    } 
    else if (payload[0] == 'E' && (payload[1] == '\0' || payload[1] == '\r' || payload[1] == '\n')) {
        setLedMode(LED_OFF);
    } 
    else if (payload[0] == 'C' && (payload[1] == '\0' || payload[1] == '\r' || payload[1] == '\n')) {
        setLedMode(LED_BLINK);
    } 
    else if (payload[0] == 'T' && (payload[1] == '\0' || payload[1] == '\r' || payload[1] == '\n')) {
        setLedMode(LED_TEMP_MONITOR);
    }
    // 2. Trames SCPI de température : :MEAS:TEMP <valeur>
    else if (strncmp(payload, ":MEAS:TEMP", 10) == 0) {
        float temp = atof(payload + 10);
        Serial.print(F("[SCPI RECU] Temperature : "));
        Serial.print(temp, 2);
        Serial.println(F(" °C"));

        if (currentMode == LED_TEMP_MONITOR) {
            if (temp > 25.0f) {
                digitalWrite(Led, HIGH);
                Serial.println(F(" -> Seuil depasse (>25°C) : LED ON"));
            } else {
                digitalWrite(Led, LOW);
                Serial.println(F(" -> Temperature nominale (<=25°C) : LED OFF"));
            }
        }
    } 
    else {
        // Affichage transparent des données inconnues
        Serial.print(F("[RX RAW] "));
        Serial.println(payload);
    }
}

void handleLedBlink() {
    if (currentMode == LED_BLINK) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousBlinkMillis >= blinkInterval) {
            previousBlinkMillis = currentMillis;
            blinkState = !blinkState;
            digitalWrite(Led, blinkState ? HIGH : LOW);
        }
    }
}

void setup() {
    Serial.begin(serial_baud_rate);
    printf_begin();

    pinMode(Led, OUTPUT);
    digitalWrite(Led, LOW);

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
    radio.startListening();

    Serial.println(F("--- Recepteur NRF24 pret ---"));
    Serial.println(F("Commandes : A (ON), E (OFF), C (Clignotement), T (Surveillance temp >25°C)"));
}

void loop() {
    // 1. Réception RF24
    if (radio.available()) {
        uint8_t bytesReceived = radio.getDynamicPayloadSize();
        if (bytesReceived == 0) {
            bytesReceived = radio.getPayloadSize();
        }
        if (bytesReceived > 32) bytesReceived = 32;

        radio.read(rxBuffer, bytesReceived);
        rxBuffer[bytesReceived] = '\0'; // Fin de chaîne sécurisée

        // Nettoyage des fins de ligne
        while (bytesReceived > 0 && (rxBuffer[bytesReceived - 1] == '\r' || rxBuffer[bytesReceived - 1] == '\n')) {
            rxBuffer[--bytesReceived] = '\0';
        }

        processCommand(rxBuffer);
    }

    // 2. Gestion non bloquante du mode clignotant
    handleLedBlink();
}