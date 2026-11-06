#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define serial_baud_rate 115200

#define CE_PIN 7
#define CSN_PIN 8
#define SERVO_PIN A1

RF24 radio(CE_PIN, CSN_PIN);
Servo servoS3003;
const unsigned long servoCommandTimeout = 500;
unsigned long lastServoCommandMillis = 0;
bool servoStoppedByTimeout = true;

const uint64_t addresses[2] = {0xa1a1a1a1a1, 0xd1d1d1d1d1};

// Configuration du nœud récepteur
bool radioNumber = 0; 
const uint8_t Led = 2;

// Machine à états pour la LED
enum LedMode {
    LED_OFF,
    LED_ON,
    LED_BLINK,
    LED_ENOCEAN_BUTTON,
    LED_TEMP_MONITOR
};

LedMode currentMode = LED_OFF;

// Gestion du clignotement non bloquant
unsigned long previousBlinkMillis = 0;
const unsigned long blinkInterval = 250; // demi-période (ms)
bool blinkState = false;

// Buffer de réception radio
char rxBuffer[33];
char servoRxBuffer[8];
uint8_t servoRxLength = 0;
unsigned long lastServoByteMillis = 0;
bool servoRxActive = false;

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
        case LED_ENOCEAN_BUTTON:
            Serial.println(F("[MODE] Bouton EnOcean actif (B)"));
            break;
        case LED_TEMP_MONITOR:
            Serial.println(F("[MODE] Surveillance temperature active (T)"));
            break;
    }
}

void toggleLed() {
    digitalWrite(Led, digitalRead(Led) == HIGH ? LOW : HIGH);
    currentMode = LED_ENOCEAN_BUTTON;
    Serial.println(F("[LED] Toggle applique, etat conserve jusqu'a une autre commande"));
}

void processCommand(const char* payload) {
    // Commande servo : S:<angle>, avec un angle compris entre 0 et 180.
    if (payload[0] == 'S' && payload[1] == ':') {
        char* end = nullptr;
        long angle = strtol(payload + 2, &end, 10);

        Serial.print(F("[SERVO RECU] Angle : "));
        Serial.println(payload + 2);

        if (end != payload + 2 && *end == '\0' && angle >= 0 && angle <= 180) {
            if (!servoS3003.attached()) {
                servoS3003.attach(SERVO_PIN);
            }
            servoS3003.write((int)angle);
            lastServoCommandMillis = millis();
            servoStoppedByTimeout = false;
            Serial.print(F("[SERVO APPLIQUE] Angle : "));
            Serial.println(angle);
        } else {
            Serial.println(F("[SERVO IGNORE] Commande invalide ou angle hors limites"));
        }
    }
    // Commandes directes sur un caractère
    else if (payload[0] == 'A' && (payload[1] == '\0' || payload[1] == '\r' || payload[1] == '\n')) {
        setLedMode(LED_ON);
    } 
    else if (payload[0] == 'E' && (payload[1] == '\0' || payload[1] == '\r' || payload[1] == '\n')) {
        setLedMode(LED_OFF);
    } 
    else if (payload[0] == 'C' && (payload[1] == '\0' || payload[1] == '\r' || payload[1] == '\n')) {
        setLedMode(LED_BLINK);
    } 
    else if (payload[0] == 'B' && (payload[1] == '\0' || payload[1] == '\r' || payload[1] == '\n')) {
        toggleLed();
    }
    // 2. Trames SCPI de température : :MEAS:TEMP <valeur>
    else if (strncmp(payload, ":MEAS:TEMP", 10) == 0) {
        float temp = atof(payload + 10);
        Serial.print(F("[SCPI RECU] Temperature : "));
        Serial.print(temp, 2);
        Serial.println(F(" °C"));

        if (currentMode == LED_TEMP_MONITOR) {
            if (temp > 26.0f) {
                digitalWrite(Led, HIGH);
                Serial.println(F(" -> Seuil depasse (>25°C) : LED ON"));
            } else {
                digitalWrite(Led, LOW);
                Serial.println(F(" -> Temperature nominale (<=26°C) : LED OFF"));
            }
        }
    } 
    else {
        // Affichage transparent des données inconnues
        Serial.print(F("[RX RAW] "));
        Serial.println(payload);
    }
}

void resetServoRxBuffer() {
    servoRxLength = 0;
    servoRxActive = false;
    servoRxBuffer[0] = '\0';
}

void flushServoRxBuffer() {
    if (servoRxActive && millis() - lastServoByteMillis >= 100 && servoRxLength > 2) {
        servoRxBuffer[servoRxLength] = '\0';
        processCommand(servoRxBuffer);
        resetServoRxBuffer();
    }
}

void processReceivedPayload(const char* payload) {
    size_t payloadLength = strlen(payload);

    // Certains emetteurs RF envoient S, :, puis chaque chiffre dans un payload distinct.
    if (!servoRxActive && payloadLength == 1 && payload[0] == 'S') {
        servoRxBuffer[0] = 'S';
        servoRxLength = 1;
        servoRxActive = true;
        lastServoByteMillis = millis();
        return;
    }

    if (servoRxActive) {
        bool validContinuation = servoRxLength == 1 ? payload[0] == ':' : true;
        size_t firstDigit = servoRxLength == 1 ? 1 : 0;
        for (size_t index = firstDigit; validContinuation && index < payloadLength; ++index) {
            if (payload[index] < '0' || payload[index] > '9') {
                validContinuation = false;
            }
        }

        if (validContinuation && servoRxLength + payloadLength < sizeof(servoRxBuffer)) {
            memcpy(servoRxBuffer + servoRxLength, payload, payloadLength);
            servoRxLength += payloadLength;
            lastServoByteMillis = millis();
            return;
        }

        if (servoRxLength > 2) {
            servoRxBuffer[servoRxLength] = '\0';
            processCommand(servoRxBuffer);
        }
        resetServoRxBuffer();
    }

    processCommand(payload);
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
    Serial.println(F("Commandes : A (ON), E (OFF), C (Clignotement), B (toggle LED)"));
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

        processReceivedPayload(rxBuffer);
    }

    // 2. Gestion non bloquante du mode clignotant
    handleLedBlink();
    flushServoRxBuffer();

    // Arret de securite si le dernier emetteur ne donne plus signe de vie.
    if (!servoStoppedByTimeout && millis() - lastServoCommandMillis >= servoCommandTimeout) {
        servoS3003.detach();
        servoStoppedByTimeout = true;
        Serial.println(F("[SERVO ARRET] Signal PWM coupe apres 1 seconde sans trame"));
    }
}