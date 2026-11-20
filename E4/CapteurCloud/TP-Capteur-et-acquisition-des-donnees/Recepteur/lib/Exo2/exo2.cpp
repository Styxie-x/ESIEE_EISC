
#include <Arduino.h>
#include <SPI.h>
#include "RF24.h"
#include <printf.h>
#include "exo2.h"

#define CE_PIN 7
#define CSN_PIN 8

static RF24 radio(CE_PIN, CSN_PIN);

void exo2() {
  Serial.begin(115200);
  printf_begin(); //pour utiliser la macro printf

  radio.NRF24Initialization(); // Start up the radio

  Serial.println("\n** Réinitialisation du module nRF24 **");

  radio.resestNRF24(); //reinitialiser les parametres NRF24

  Serial.println("\n** Paramètres MAC et PHY **");

  radio.printDetails();
}
