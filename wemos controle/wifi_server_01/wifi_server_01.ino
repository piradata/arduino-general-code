/*  Connects to the home WiFi network
    Asks some network parameters
    Starts WiFi server with fix IP and listens
    Receives and sends messages to the client
    Communicates: wifi_client_01.ino
*/
#include <SPI.h>
#include <ESP8266WiFi.h>

#define LEDPIN 2

String message;
String texto;
int c;
int input;
int output;

char ssid[] = "PIRADATA";                 // SSID of your home WiFi
char pass[] = "guifernandabf";           // password of your home WiFi
WiFiServer server(80);

IPAddress ip(192, 168, 43, 21);           // IP address of the server
IPAddress gateway(192, 168, 43, 1);       // gateway of your network
IPAddress subnet(255, 255, 255, 0);      // subnet mask of your network

void setup() {
  Serial.begin(115200);                    // only for debug
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, pass);                // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.begin();                        // starts the server
  Serial.println("Connected to wifi");
  Serial.print("Status: "  ); Serial.println(WiFi.status()       );  // some parameters from the network
  Serial.print("IP: "      ); Serial.println(WiFi.localIP()      );
  Serial.print("Subnet: "  ); Serial.println(WiFi.subnetMask()   );
  Serial.print("Gateway: " ); Serial.println(WiFi.gatewayIP()    );
  Serial.print("SSID: "    ); Serial.println(WiFi.SSID()         );
  Serial.print("Signal: "  ); Serial.println(WiFi.RSSI()         );
  Serial.print("Networks: "); Serial.println(WiFi.scanNetworks() );
  pinMode(LEDPIN, OUTPUT);
}

void loop () {
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      digitalWrite(LEDPIN, LOW);  // to show the communication only (inverted logic)

      if (Serial.available() > 0) {
        c = Serial.read();
        while (c != 10) {
          texto += String((char)c);
          while (!(Serial.available() > 0));
          c = Serial.read();
        }
        client.println(texto); // sends the message to the client
        texto = "";
      }

      message = client.readStringUntil('\n');   // receives the message from the client
      if (!message.equals("\n") and !message.equals("\r") and !message.equals("\r\n") and !message.equals("")) {
        Serial.println("from client: \"" + message + "\"");
      }

      digitalWrite(LEDPIN, HIGH);
    }
    //client.stop();                // tarminates the connection with the client
  }
}
