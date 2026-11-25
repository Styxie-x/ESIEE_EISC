#include <Arduino.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Widget.h"
#include <Adafruit_SSD1306.h>

// Le canal, le débit, l’adresse et la taille de trame doivent correspondre à l’autre carte.
const uint8_t RADIO_CE = 7;
const uint8_t RADIO_CSN = 8;
const uint8_t RADIO_CHANNEL = 30;
const rf24_datarate_e RADIO_RATE = RF24_2MBPS;
const uint64_t STATION_ADDRESS = 0xd1d1d1d1d1ULL;
const uint8_t RADIO_PAYLOAD_SIZE = 32;

const uint8_t OLED_WIDTH = 128;
const uint8_t OLED_HEIGHT = 64;
const uint8_t OLED_ADDRESS = 0x3C;
const int8_t OLED_RESET = -1;
const uint32_t DISPLAY_PERIOD_MS = 3000;

// Le format radio emploie les mêmes types et le même ordre des octets sur les deux Uno.
const uint32_t MEASUREMENT_MAGIC = 0x314D4854UL; // Octets ASCII THM1
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
Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

float TEMPERATURE = 0.0f;
float HUMIDITY = 0.0f;

Gauge gaugeTemperature(&oled, 64, 58, 48);
Gauge_hum gaugeHumidity(&oled);

void displayOled() {
    static uint8_t widget = 0;
    static uint32_t lastDisplay = 0;

    if (millis() - lastDisplay < DISPLAY_PERIOD_MS) {
        return;
    }
    lastDisplay = millis();

    // Effacement complet avant chaque page.
    oled.clearDisplay();
    oled.display();

    if (widget == 0) {
        gaugeTemperature.draw();
        widget = 1;
    } else {
        gaugeHumidity.draw();
        widget = 0;
    }

    oled.display();
}

void setup() {
    Serial.begin(115200);
    Wire.begin();
    if (oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        oled.clearDisplay();
        oled.setTextSize(1);
        oled.setTextColor(SSD1306_WHITE);
        oled.setCursor(4, 12);
        oled.print(F("En attente..."));
        oled.display();
    } else {
        Serial.println(F("[RX] OLED 128x64 introuvable"));
    }

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
    displayOled();

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

    TEMPERATURE = measurement.temperatureCentiC / 100.0f;
    HUMIDITY = measurement.humidityCentiPct / 100.0f;

    Serial.print(measurement.sequence);
    Serial.print(',');
    Serial.print(TEMPERATURE, 2);
    Serial.print(',');
    Serial.println(HUMIDITY, 2);
}