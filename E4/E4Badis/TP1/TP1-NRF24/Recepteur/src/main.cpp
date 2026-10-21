#include <Arduino.h>
/*
 * Ce code permet d'envoyer des données d'un noeud à un autre
 * Mise à jours : Septembre 20 par Hakim Badis
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

char message;

#define serial_baud_rate 115200

//****** CE and CSN are configurable, specified values for Arduino NANO as connected above
#define CE_PIN 7
#define CSN_PIN 8

RF24 radio(CE_PIN, CSN_PIN);

// byte addresse[6] = "1Node";
const uint64_t addresses[2] = {0xa1a1a1a1a1, 0xd1d1d1d1d1};

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1 to distinguish the two nodes  ***/
bool radioNumber = 0;

// Used to control whether this node is sending or receiving
bool role = 0;
const uint8_t Led = 2; // LED integree du Nano, broche D13

void setup()
{
  Serial.begin(115200);
  printf_begin();
  pinMode(Led, OUTPUT);
  digitalWrite(Led, LOW);

  radio.NRF24Initialization();
  radio.FastresestNRF24();
  radio.setRetries(5, 15);
  radio.setDataRate(RF24_2MBPS);
  radio.setChannel(30);
  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_HIGH);

  // Open a writing and reading pipe on each radio, with opposite addresses
  if (radioNumber)
  {
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1, addresses[0]);
  }
  else
  {
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
  }

  radio.printDetails();

  Serial.println(F("\n*** PRESS 'T' to begin transmitting to the other node*****\n"));

  // Start the radio listening for data
  radio.startListening();
  digitalWrite(Led, HIGH);

}

void loop()
{
  /****************** Pong Back Role ***************************/

  radio.startListening();

  if (radio.available())
  {
    // Variable for the received timestamp
    while (radio.available())
    { // While there is data ready
      radio.read(&message, sizeof(char)); // Get the payload
      

      if (message == 'A')
      {
        digitalWrite(Led, HIGH);
      }
      else if (message == 'E')
      {
        digitalWrite(Led, LOW);
      }
      else if (message == 'B')
      {
        digitalWrite(Led, HIGH);
        delay(250);
        digitalWrite(Led, LOW);
      }

      else
      {
        Serial.print(message);
      }
    }
  }
}
