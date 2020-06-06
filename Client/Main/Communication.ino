String msg;

void getMessage() {
  while (client.available() > 0) {
    char c = client.read();
    if (c == '\n') {
      onDeviceCommand(getValue(msg, '|', 0));
      msg = "";
    } else {
      msg = msg + c;
    }
  }
}

void sendMessage(String msg) {
  Serial.println("Device -> " + msg) ;
  client.write((msg + '\n').c_str());
}
