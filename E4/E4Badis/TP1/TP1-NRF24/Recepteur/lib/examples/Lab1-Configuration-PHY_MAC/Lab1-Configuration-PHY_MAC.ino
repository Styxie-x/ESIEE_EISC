
/*
** Mise à jours : Septembre 2016 par Hakim Badis
*/

#include <SPI.h>
#include "RF24.h"
#include <printf.h>

#define serial_baud_rate           115200

//****** CE and CSN are configurable, specified values for Arduino NANO as connected above

#define CE_PIN 7
#define CSN_PIN 8

RF24 radio(CE_PIN, CSN_PIN);

void setup() {
  Serial.begin(115200);
  printf_begin(); //pour utiliser la macro printf
  
  radio.NRF24Initialization(); // Start up the radio

  Serial.println("\n** Réinitialisation du module nRF24 **");

  radio.resestNRF24(); //reinitialiser les parametres NRF24

  Serial.println("\n** Paramètres MAC et PHY **");

  radio.printDetails();
}

void loop() {


} // Loop

