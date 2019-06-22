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
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>              // include libraries
#include <LoRa.h>

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#define csPin D3          // LoRa radio chip select
#define resetPin D2       // LoRa radio reset
#define irqPin D1         // change for your board; must be a hardware interrupt pin

#define sclk D5
#define mosi D7
#define cs   D8
#define rst  D0
#define dc   D4

String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xBB;     // address of this device
byte destination = 0xFF;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends

Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, rst);

float p = 3.1415926;

void setup() {
  Serial.begin(115200);                   // initialize serial
  while (!Serial);

  display.begin();

  display.fillScreen(BLACK);
  display.setCursor(0, 0);
  display.println("LoRa Duplex");
  delay(1000);

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(csPin, resetPin, irqPin);// set CS, reset, IRQ pin

  while (!LoRa.begin(433E6)) { // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
  }
  Serial.println("LoRa init succeeded.");

  display.fillScreen(BLACK);
  display.setCursor(0, 0);
  display.println("LoRa Duplex\nSuccess");
  delay(500);
}



void loop() {
  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID

  byte incoming1Length = LoRa.read();    // incoming msg length
  String incoming1 = "";
  for (byte temp = incoming1Length; temp >= 1; temp = temp - 1) {
    incoming1 += (char)LoRa.read();
  }

  byte incoming2Length = LoRa.read();    // incoming msg length
  String incoming2 = "";
  for (byte temp = incoming2Length; temp >= 1; temp = temp - 1) {
    incoming2 += (char)LoRa.read();
  }

  if (incoming1Length != incoming1.length()) {   // check length for error
    display.fillScreen(BLACK);
    display.setCursor(0, 0);
    display.println("error: message length does not match length");
    delay(10);
    return;                             // skip rest of function
  }

  if (incoming2Length != incoming2.length()) {   // check length for error
    display.fillScreen(BLACK);
    display.setCursor(0, 0);
    display.println("error: message length does not match length");
    delay(10);
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    display.fillScreen(BLACK);
    display.setCursor(0, 0);
    display.println("This message is not for me.");
    delay(10);
    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  Serial.println("GyroZ = " + incoming1 + "g/s");
  Serial.println("GrausZ = " + incoming2 + "g");
  Serial.println();

  display.fillScreen(BLACK);
  display.setCursor(0, 0);
  if(incoming1[0]!='-')display.println("\n\nGyroZ= +" + incoming1 + "g/s");
  else display.println("\n\nGyroZ= " + incoming1 + "g/s");
  if(incoming2[0]!='-')display.println("\n\nGrauZ= " + incoming2 + "g");
  else display.println("\n\nGrauZ= " + incoming2 + "g");
  delay(10);
}

