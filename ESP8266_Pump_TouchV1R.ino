const int touchSensorPin = D2; // The pin where touch sensor is connected
const int relayPin = D1; // The pin where relay is connected

void setup() {
  pinMode(touchSensorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Relay is off at start
}

void loop() {
  int sensorState = digitalRead(touchSensorPin);
  if (sensorState == HIGH) {
    digitalWrite(relayPin, LOW); // turn relay ON
  } else {
    digitalWrite(relayPin, HIGH); // turn relay OFF
  }
}
