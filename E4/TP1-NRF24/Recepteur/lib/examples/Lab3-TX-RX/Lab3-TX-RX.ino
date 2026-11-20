
/*
* Ce code permet d'envoyer des données d'un noeud à un autre
* Mise à jours : Septembre 2016 par Hakim Badis
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define serial_baud_rate           115200

//****** CE and CSN are configurable, specified values for Arduino NANO as connected above
#define CE_PIN 7
#define CSN_PIN 8

RF24 radio(CE_PIN, CSN_PIN);

//byte addresse[6] = "1Node";
const uint64_t addresses[2] = {0xa1a1a1a1a1, 0xd1d1d1d1d1} ;

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1 to distinguish the two nodes  ***/
bool radioNumber = 0;

// Used to control whether this node is sending or receiving
bool role = 0;

void setup() {
  Serial.begin(115200);
  printf_begin();
   
  radio.NRF24Initialization();
  radio.FastresestNRF24();
  radio.setRetries(5,15);
  radio.setDataRate( RF24_2MBPS ) ;
  radio.setChannel(50);
  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_MAX);

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
}

void loop() {
  
  
/****************** Ping Out Role ***************************/  
if (role == 1)  {
    
    radio.stopListening();                                    // First, stop listening so we can talk.
    
    
    Serial.println(F("Now sending"));

    unsigned long time = micros();                             // Take the time, and send it.  This will block until complete
     if (!radio.write( &time, sizeof(unsigned long) )){
       Serial.println(F("failed"));
     }
        
    radio.startListening();                                    // Now, continue listening
    
    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){                                             // Describe the results
        Serial.println(F("Failed, response timed out."));
    }else{
        unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
        radio.read( &got_time, sizeof(unsigned long) );
        unsigned long time = micros();
        
        // Spew it
        Serial.print(F("Sent "));
        Serial.print(time);
        Serial.print(F(", Got response "));
        Serial.print(got_time);
        Serial.print(F(", Round-trip delay "));
        Serial.print(time-got_time);
        Serial.println(F(" microseconds"));
    }

    // Try again 1s later
    delay(1000);
  }



/****************** Pong Back Role ***************************/

  if ( role == 0 )
  {
    unsigned long got_time;
    
    if( radio.available()){
                                                                    // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &got_time, sizeof(unsigned long) );             // Get the payload
      }
     
      radio.stopListening();                                        // First, stop listening so we can talk   
      radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.      
      radio.startListening();                                       // Now, resume listening so we catch the next packets.     
      Serial.print(F("Sent response "));
      Serial.println(got_time);  
   }
 }




/****************** Change Roles via Serial Commands ***************************/

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == 0 ){      
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = 1;                  // Become the primary transmitter (ping out)
    
   }else
    if ( c == 'R' && role == 1 ){
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));      
       role = 0;                // Become the primary receiver (pong back)
       radio.startListening();
       
    }
  }


} // Loop

