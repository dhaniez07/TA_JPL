#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

#define MAX_SRV_CLIENTS 8
const char* ssid = "JPL";
const char* password = "12341234";

WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];



void setup() {
  Serial.begin(115200);
  Serial.println("\nConnecting");

  wifiMulti.addAP(ssid, password);

  Serial.println("Connecting Wifi ");
  for (int loops = 10; loops > 0; loops--) {
    if (wifiMulti.run() == WL_CONNECTED) {
      Serial.println("");
      Serial.print("WiFi connected ");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      break;
    }
    else {
      Serial.println(loops);
      delay(1000);
    }
  }
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("WiFi connect failed");
    delay(1000);
    ESP.restart();
  }

  //start UART and the server
  Serial2.begin(9600);
  server.begin();
  server.setNoDelay(true);
  Serial.print("Ready! Use 'telnet ");
  Serial.print(WiFi.localIP());
  Serial.println(" 23' to connect");
}

void loop() {
  uint8_t i;
  if (wifiMulti.run() == WL_CONNECTED) {
    if (server.hasClient()) {
      for (i = 0; i < MAX_SRV_CLIENTS; i++) {
        if (!serverClients[i] || !serverClients[i].connected()) {
          serverClients[i] = server.available();
          serverClients[i].print("TEST"); Serial.print("New client: ");
          Serial.print(i); Serial.print(' ');
          Serial.println(serverClients[i].remoteIP());
          break;
        }
      }
      if (i >= MAX_SRV_CLIENTS) {
        server.available().stop();
      }
    }
  }
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i].available() > 0) {
      char c = serverClients[i].read();
      Serial.write(c);
      for (int x = 0; x < MAX_SRV_CLIENTS; x++) {
        if (serverClients[i] != serverClients[x]) {
          serverClients[x].write(c);
        }
      }
    }
  }
}
