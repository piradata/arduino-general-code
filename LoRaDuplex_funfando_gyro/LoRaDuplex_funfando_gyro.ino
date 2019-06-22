/*
  LoRa Duplex communication

  Sends a message every half second, and polls continually
  for new incoming messages. Implements a one-byte addressing scheme,
  with 0xFF as the broadcast address.

  Uses readString() from Stream class to read payload. The Stream class'
  timeout may affect other functuons, like the radio's callback. For an

  created 28 April 2017
  by Tom Igoe
*/
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

#include <SPI.h>              // include libraries
#include <LoRa.h>

#define csPin D3          // LoRa radio chip select
#define resetPin D4 //D2      // LoRa radio reset
#define irqPin D8   //D1      // change for your board; must be a hardware interrupt pin

String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xBB;     // address of this device
byte destination = 0xFF;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends

long timer = 0;

void setup() {
  Serial.begin(115200);                   // initialize serial
  while (!Serial);

  Wire.begin();
  mpu6050.begin();
  //mpu6050.calcGyroOffsets(true);
  mpu6050.setGyroOffsets(-2.1, 0.3, 1.44);

  Serial.println("LoRa Duplex");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(csPin, resetPin, irqPin);// set CS, reset, IRQ pin

  while(!LoRa.begin(433E6)) {  // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
  }
  Serial.println("LoRa init succeeded.");
}

void loop() {
  mpu6050.update();

if(millis() - timer > 5){
    
    Serial.println("=======================================================");

    Serial.print("\tgyroZ : ");Serial.println(mpu6050.getGyroZ());

    Serial.print("\tangleZ : ");Serial.println(mpu6050.getAngleZ());
    Serial.println("=======================================================\n");


    String message1 = String(mpu6050.getGyroZ());   // send a message
    String message2 = String(mpu6050.getAngleZ());
    sendMessage(message1,message2);
    Serial.println("Sending gyroz = " + message1 + " and anglez = " + message2);
    
    timer = millis();
  }

}

void sendMessage(String outgoing1, String outgoing2) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing1.length());        // add payload length
  LoRa.print(outgoing1);                 // add payload
  LoRa.write(outgoing2.length());        // add payload length
  LoRa.print(outgoing2);                 // add payload
  
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
}
