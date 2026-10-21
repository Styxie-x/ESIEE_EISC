#include <Arduino.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define serial_baud_rate           115200

//****** CE and CSN are configurable, specified values for Arduino NANO as connected above
#define CE_PIN 7
#define CSN_PIN 8
    char c;

RF24 radio(CE_PIN, CSN_PIN);

//byte addresse[6] = "1Node";
const uint64_t addresses[2] = {0xa1a1a1a1a1, 0xd1d1d1d1d1} ;

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1 to distinguish the two nodes  ***/
bool radioNumber = 1;

// Used to control whether this node is sending or receiving
int Led = 2;


void setup() {
  Serial.begin(115200);
  printf_begin();
   
  radio.NRF24Initialization();
  radio.FastresestNRF24();
  radio.setRetries(5,15);
  radio.setDataRate( RF24_2MBPS ) ;
  radio.setChannel(30);
  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_HIGH);

  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }

   radio.printDetails();

  Serial.println(F("\n*** PRESS 'T' to begin transmitting to the other node*****\n"));


  // Start the radio listening for data
  radio.startListening();

  

  pinMode(Led, OUTPUT); //L1 est une broche de sortie
}


void loop() {

  radio.stopListening();

  if (Serial.available()) {

    c = Serial.read();

    Serial.println(c);

    radio.write(&c, sizeof(char));

  }
}


/****************** Pong Back Role ***************************/
