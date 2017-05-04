// ESP8266 WiFi <-> UART Bridge
// by Luca Remigio

#define UART_BAUD 9600
#define directionPin 0   //Settarlo su HIGH in invio e LOW in ricezione
// ESP WiFi mode:


const char *ssid = "xxxx";  // Your ROUTER SSID
const char *pw = "xxx"; // and WiFi PASSWORD
const int port = 8888;

byte ip[] = { 192, 168, 0, 70 };
byte gateway[] = { 192, 168, 0, 1 };
byte subnet[] = { 255, 255, 255, 0 };
//////////////////////////////////////////////////




#include <ESP8266WiFi.h>
#include <WiFiClient.h>


WiFiServer server(port);
WiFiClient client;


uint8_t buf1[1024];
uint8_t i1=0;

uint8_t buf2[1024];
uint8_t i2=0;



void setup() {

  delay(500);

  Serial.begin(UART_BAUD);

  WiFi.config(ip,gateway,subnet);
  WiFi.begin(ssid, pw);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  pinMode(directionPin, OUTPUT);

  server.begin(); // start TCP server
}


void loop() {

  if(!client.connected()) { // if client not connected
    client = server.available(); // wait for it to connect
    return;
  }

  // here we have a connected client

  if(client.available()) {
	  digitalWrite(directionPin,LOW);  //Impostazione protocollo in ricezione
	  while(client.available()) {
		  buf1[i1] = (uint8_t)client.read(); // read char from client (RoboRemo app)
		  if(i1<1023) i1++;
	  }
    // now send to UART:
    Serial.write(buf1, i1);
    i1 = 0;
  }

  if(Serial.available()) {
	  digitalWrite(directionPin,HIGH);  //Impostazione protocollo in invio
		while(Serial.available()) {
		  buf2[i2] = (char)Serial.read(); // read char from UART
		  if(i2<1023) i2++;
		}
		// now send to WiFi:
		client.write((char*)buf2, i2);

		i2 = 0;
  }

}
