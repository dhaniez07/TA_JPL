#include <WiFi.h>
#include <WiFiAP.h>
const char* ssid = "JPL";
const char* password = "12341234";
WiFiServer server (42);
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(5,OUTPUT);

  Serial.println();
  Serial.println ("Membuat jaringan");
  WiFi.softAP(ssid,password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("IP : ");
  Serial.println(myIP);
  Serial.print("ssid : ");
  Serial.println(ssid);
  Serial.print("Password :");
  Serial.println(password);
  server.begin();
  Serial.println("Server dijalankan");
  
}

void loop() {
  }
