/*************************************************************************
 * Remote node - nRF24L01+ radio communications                          *
 *      A program to operate a remote-node slave device that sends       *
 *      data to a command unit on a given request message. The radio     *            
 *      transceiver used is the nRF24L01+, and it operates using the     *
 *      TMRh20 RF24 library.                                             *
 *                                                                       *
 *      Author: B.D. Fraser                                              *
 *                                                                       *
 *        Last modified: 27/06/2017                                      *
 *                                                                       *
 *************************************************************************/

// nRF24L01 radio transceiver external libraries
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <printf.h>

// chip select and RF24 radio setup pins
#define CE_PIN 9
#define CSN_PIN 10
RF24 radio(CE_PIN,CSN_PIN);

// setup radio pipe addresses for each sensor node
const byte nodeAddress[5] = {'N','O','D','E','1'};

// simple integer array for remote node data, in the form [node_id, returned_count]
int remoteNodeData[2] = {1, 1,};

// integer to store count of successful transmissions
int dataFromMaster = 0;


/* Function: setup
 *    Initialises the system wide configuration and settings prior to start
 */
void setup() {

  Serial.begin(9600);

  // ----------------------------- RADIO SETUP CONFIGURATION AND SETTINGS -------------------------// 
  
  radio.begin();
  
  // set power level of the radio
  radio.setPALevel(RF24_PA_LOW);

  // set RF datarate
  radio.setDataRate(RF24_250KBPS);

  // set radio channel to use - ensure it matches the target host
  radio.setChannel(0x66);

  // open a reading pipe on the chosen address - matches the master tx
  radio.openReadingPipe(1, nodeAddress);     

  // enable ack payload - slave replies with data using this feature
  radio.enableAckPayload();

  // preload the payload with initial data - sent after an incoming message is read
  radio.writeAckPayload(1, &remoteNodeData, sizeof(remoteNodeData));

  // print radio config details to console
  printf_begin();
  radio.printDetails();

  // start listening on radio
  radio.startListening();
  
  // --------------------------------------------------------------------------------------------//
}


/* Function: loop
 *    main loop program for the slave node - repeats continuously during system operation
 */
void loop() {
  
  // transmit current preloaded data to master device if message request received
  radioCheckAndReply();
}


/* Function: updateNodeData
 *    updates the count variable for the node and stores it in the nRF24L01+ radio
 *    preloaded ack payload ready for sending on next received message
 */
void updateNodeData(void) 
{
  // increment node count - reset to 1 if exceeds 500
  if (remoteNodeData[1] < 500) {
    remoteNodeData[1]++;
  } else {
    remoteNodeData[1] = 1;
  }

  // set the ack payload ready for next request from the master device
  radio.writeAckPayload(1, &remoteNodeData, sizeof(remoteNodeData));
}


/* Function: radioCheckAndReply
 *    sends the preloaded node data over the nrf24l01+ radio when
 *    a message is received by the master
 */
void radioCheckAndReply(void)
{
    // check for radio message and send sensor data using auto-ack
    if ( radio.available() ) {
          radio.read( &dataFromMaster, sizeof(dataFromMaster) );
          Serial.println("Received request from master - sending preloaded data.");

          // update the node count after sending ack payload - provides continually changing data
          updateNodeData();
    }
}
