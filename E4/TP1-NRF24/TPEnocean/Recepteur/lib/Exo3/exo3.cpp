
// #include <Arduino.h>
// #include <SPI.h>
// #include "RF24.h"
// #include <printf.h>
// #include "exo3.h"

// #define CE_PIN 7
// #define CSN_PIN 8

// static RF24 radio(CE_PIN, CSN_PIN);

// void exo3() {
//   Serial.begin(115200);
//   printf_begin(); //pour utiliser la macro printf

//   radio.NRF24Initialization(); // Start up the radio
//   radio.FastresestNRF24();

//   Serial.println("\n** Configuration par default **");
//   radio.printDetails();

//   Serial.println("\n** Configuration actuelle **");
//   radio.printDetails();

//   radio.openWritingPipe(0xa1a1a1a1a0);
//   radio.openReadingPipe(1, 0xa1a1a1a1a0);
//   radio.
//   radio.enableDynamicPayloads();
//   radio.enableAckPayload();

//   // This device is the TX

//   // Start the radio listening for data
//   //radio.startListening();
// }

// void exo3Loop() {
// }