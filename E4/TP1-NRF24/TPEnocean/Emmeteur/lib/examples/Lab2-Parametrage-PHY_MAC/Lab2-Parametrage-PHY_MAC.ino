
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

uint64_t addresses[3] = {0xa1a1a1a1a0, 0xa1a1a1a1a1, 0xa1a1a1a1a2} ;



void setup() {
  
  Serial.begin(115200);
  
  printf_begin(); //pour utiliser la macro printf
  
  radio.NRF24Initialization(); // Start up the radio

  radio.FastresestNRF24();
  
 Serial.println("\n** Configuration par default **");
 
  radio.printDetails();


  
 Serial.println("\n** Configuration actuelle **");
 
  radio.printDetails();
  
  // Start the radio listening for data
  //radio.startListening();
}

void loop() {


} // Loop

