#include <WiFi.h>

const char* ssid     = "JPL";
const char* password = "12341234";
IPAddress server(192, 168, 4, 2);
WiFiClient client;

const int sensorA1 = 36;
const int sensorA2 = 39;
const int sensorB1 = 34;
const int sensorB2 = 35;
const int mswitch1 = 6;
const int mswitch2 = 7;
const int axlA = 12;
const int axlB = 13;
const int motor1 = 16;
const int motor2 = 17;
const int lamp1 = 10;
const int lamp2 = 11;

int kondisi = 0;
int count = 0;

void setup() {
  pinMode(sensorA1, INPUT);
  pinMode(sensorA2, INPUT);
  pinMode(sensorB1, INPUT);
  pinMode(sensorB2, INPUT);
  pinMode(mswitch1, INPUT);
  pinMode(mswitch2, INPUT);
  pinMode(axlA, INPUT_PULLUP);
  pinMode(axlB, INPUT_PULLUP);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(lamp1, OUTPUT);
  pinMode(lamp2, OUTPUT);

  digitalWrite(sensorA1, HIGH);
  digitalWrite(sensorA2, HIGH);
  digitalWrite(sensorB1, HIGH);
  digitalWrite(sensorB2, HIGH);
  digitalWrite(mswitch1, HIGH);
  digitalWrite(mswitch2, HIGH);
  digitalWrite(axlA, HIGH);
  digitalWrite(axlB, HIGH);
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.connect(server, 23);
  if (client.connected() > 0) {
    Serial.println("SUCCESS");
  } else {
    Serial.println("FAIL");
  }
}

int value = 0;

void JPL_B () {
  if (count == 0) {
    client.println("Train passed");
  }
  if (digitalRead(sensorA1) == LOW) {
    client.println("Train east|");
  }
  if (digitalRead(sensorB1) == LOW) {
    client.println("Train east|");
  }
  if (digitalRead(mswitch1) == LOW) {
    kondisi = 0;
    if (count == 0) {
      kondisi = 2;
    }
  }
  if (digitalRead(mswitch2) == LOW) {
    kondisi = 4;
  }
  if (digitalRead(motor1) == HIGH) {
    if (digitalRead(axlA) == LOW) {
      count++;
      Serial.println("Jumlah :");
      Serial.println(count);
      delay(200);
    }
    if (digitalRead(axlB) == LOW) {
      count--;
      Serial.println("Jumlah :");
      Serial.println(count);
      delay(200);
    }
  }
  if (digitalRead(mswitch1) == LOW) {
    if (digitalRead(axlA) == LOW) {
      count++;
      Serial.println("Jumlah :");
      Serial.println(count);
      delay(200);
    }
    if (digitalRead(axlB) == LOW) {
      count--;
      Serial.println("Jumlah :");
      Serial.println(count);
      delay(200);
    }
  }
  switch (kondisi) {
    case 0 :
      digitalWrite(motor1, LOW);
      break;
    case 1 :
      digitalWrite(motor1, HIGH);
      digitalWrite(motor2, LOW);
      client.println("Gate A Close|");
      break;
    case 2 :
      digitalWrite(motor2, HIGH);
      digitalWrite(motor1, LOW);
      client.println("Gate A Open|");
      break;
    case 3 :
      digitalWrite(motor2, HIGH);
      digitalWrite(motor1, LOW);
      break;
    case 4 :
      digitalWrite(motor2, LOW);
      digitalWrite(motor1, LOW);
      break;
  }
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String msg;
void loop() {
  while (client.available() > 0) {
    char c = client.read();
    if (c == '\n') {
      Serial.println(getValue(msg, '|', 0));
      if (getValue(msg, '|', 0) == "Train west") {
        Serial.println("Gate closing");
        kondisi = 1;
      }
      if (getValue(msg, '|', 0) == "Train passed") {
        Serial.println("Gate opening");
        kondisi = 2;
      }
      msg = "";
    } else {
      msg = msg + c;
    }
  }

  // as long as there are bytes in the serial queue,
  // read them and send them out the socket if it's open:
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    if (client.connected()) {
      client.print(inChar);
    }
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    // do nothing:
    while (true) {
      delay(1);
    }
  }
  JPL_B();
}
