#include <WiFi.h>

const char* ssid     = "JPL";
const char* password = "12341234";
IPAddress server(192, 168, 4, 2);
WiFiClient client;

int sensor_a1 = 36;
int axl1 = 32;
int axl2 = 33;
int mswitch1 = 25;
int mswitch2 = 26;
int motorUp = 17;
int motorDown = 16;

void setup() {
  Serial.begin(115200);
  initWifi();
  initTelnet();
  initPin();
}


void loop() {
  delay(10);
  onDeviceCycle();
  getMessage();
  Serial.write(digitalRead(sensor_a1) == HIGH);
  if (!client.connected()) {
    Serial.println("Disconnected");
    client.stop();
//    initWifi();
    initTelnet();
  }
}
