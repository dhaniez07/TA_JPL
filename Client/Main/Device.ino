boolean last_a1 = false;
boolean cgsBot = false;
boolean cgsTop = false;
boolean axlePlus = false;
boolean axleMin = false;
int count;

void initPin() {
  pinMode(sensor_a1, INPUT);
  pinMode(axl1, INPUT);
  pinMode(axl2, INPUT);
  pinMode(mswitch1, INPUT);
  pinMode(mswitch2, INPUT);
  pinMode(motorUp, OUTPUT);
  pinMode(motorDown, OUTPUT);
}

void onDeviceCommand(String msg) {
  Serial.println("Server -> " + msg);
  if (msg == ":RequestState") {
    onGetDeviceState(msg);
  } else if (msg == ":Open Gate") {
  } else if (msg == ":Close Gate") {
  } else if (msg == ":SetGate") {
  } else if (msg == ":SetGate") {
  } else if (msg == ":SetGate") {
  } else {
  }
}

void onDeviceCycle() {
  checkTrain();
  countIn();
  countOut();
  checkGateSensorBottom();
  checkGateSensorTop();
}

void onGetDeviceState(String msg) {
  sendMessage(String("OK"));
}

void checkTrain() {
  boolean  sensor = digitalRead(sensor_a1);
  if (last_a1 != sensor) {
    if (sensor) {
      //0 -> 1
      sendMessage("IncomingTrain EW");
      delay(100);
    } else {
      //1 -> 0
      sendMessage("OutgoingTrain EW");
      delay(100);
    }
    last_a1 = sensor;
  }
}

void countIn() {
  boolean  axle1 = digitalRead(axl1);
  if (axlePlus != axle1) {
    if (axle1) {
      //0 -> 1
      sendMessage("AxleCountUp");
      delay(100);
    } 
    axlePlus = axle1;
  }
}

void countOut() {
  boolean  axle2 = digitalRead(axl2);
  if (axleMin != axle2) {
    if (!axle2) {
      //1 -> 0
      sendMessage("AxleCountDown");
      delay(100);
    }
    axleMin = axle2;
  }
}

void checkGateSensorBottom() {
  boolean  checkBot = digitalRead(mswitch2);
  if (cgsBot != checkBot) {
    if (checkBot) {
      //0 -> 1
      sendMessage("Gate closed");
      delay(100);
    } else {
      //1 -> 0
      sendMessage("Gate opening");
      delay(100);
    }
    cgsBot = checkBot;
  }
}

void checkGateSensorTop() {
  boolean  checkTop = digitalRead(mswitch1);
  if (cgsTop != checkTop) {
    if (checkTop) {
      //0 -> 1
      sendMessage("Gate opened");
      delay(100);
    } else {
      //1 -> 0
      sendMessage("Gate closing");
      delay(100);
    }
    cgsTop = checkTop;
  }
}
