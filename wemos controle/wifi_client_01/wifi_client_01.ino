/*  Connects to the home WiFi network
   Asks some network parameters
   Sends and receives message from the server in every 2 seconds
   Communicates: wifi_server_01.ino
*/
#include <SPI.h>
#include <ESP8266WiFi.h>

#define LEDPIN 2

String message;
String texto;
int c;
int input;
int output;

char ssid[] = "PIRADATA";           // SSID of your home WiFi
char pass[] = "guifernandabf";            // password of your home WiFi

unsigned long askTimer = 0;

IPAddress server(192, 168, 43, 21);    // the fix IP address of the server
WiFiClient client;

void setup() {
  pinMode(D5, OUTPUT);
  digitalWrite(D5, LOW);
  pinMode(D6, OUTPUT);
  digitalWrite(D6, LOW);
  Serial.begin(115200);               // only for debug
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to wifi");
  Serial.print("Status: " ); Serial.println(WiFi.status()    );    // Network parameters
  Serial.print("IP: "     ); Serial.println(WiFi.localIP()   );
  Serial.print("Subnet: " ); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP() );
  Serial.print("SSID: "   ); Serial.println(WiFi.SSID()      );
  Serial.print("Signal: " ); Serial.println(WiFi.RSSI()      );
  pinMode(LEDPIN, OUTPUT);
}

void loop () {

  client.connect(server, 80);   // Connection to the server
  digitalWrite(LEDPIN, LOW);    // to show the communication only (inverted logic)

  if (Serial.available() > 0) {
    c = Serial.read();
    while (c != 10) {
      texto += String((char)c);
      while (!(Serial.available() > 0));
      c = Serial.read();
    }
    client.println(texto); // sends the message to the server
    texto = "";
  }
  message = client.readStringUntil('\n');   // receives the message from the server
  if (!message.equals("\n") and !message.equals("\r") and !message.equals("\r\n") and !message.equals("")) {
    Serial.println("from server: \"" + message + "\"");

    if (message.charAt(0) == '1') {
      // turn LED on:
      digitalWrite(D5, HIGH);
    } else {
      // turn LED off:
      digitalWrite(D5, LOW);
    }
    if (message.charAt(1) == '1') {
      // turn LED on:
      digitalWrite(D6, HIGH);
    } else {
      // turn LED off:
      digitalWrite(D6, LOW);
    }



  }
  digitalWrite(LEDPIN, HIGH);
}
