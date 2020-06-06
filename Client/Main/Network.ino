void initWifi() {
  delay(10);
  Serial.print("\nConnecting WiFi to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
}

void initTelnet() {
  client.connect(server, 23);
  if (client.connected() > 0) {
    Serial.println("Connected to Telnet Server");
  } else {
    Serial.println("Connection Failed");
  }
}
