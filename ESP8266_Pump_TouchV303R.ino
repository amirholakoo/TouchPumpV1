const int touchSensorPin = D2; // The pin where touch sensor is connected
const int relayPin = D1; // The pin where relay is connected
const int builtinLedPin = LED_BUILTIN; // Built-in LED on NodeMCU
const int secondLedPin = D0; // Second LED pin, please modify according to your wiring

unsigned long lastTouchTime = 0; // When the touch sensor was last touched
unsigned long touchStartTime = 0; // When the current touch started
unsigned long debounceDelay = 500; // Delay to debounce the touch sensor
unsigned long doubleTapInterval = 400; // How long to wait for a second tap
unsigned long touchInterruptTime = 2000; // How long to touch to interrupt the pump
unsigned long fewMinutes = 300000; // 60*1000 minutes in milliseconds
unsigned long fewSeconds = 10000; // 5*1000 seconds in milliseconds

bool lastState = LOW; // Last state of the touch sensor
int tapCount = 0; // How many times the sensor has been tapped

enum Mode {
  OFF,
  FEW_SECONDS,
  FEW_MINUTES
};

Mode currentMode = OFF;

void setup() {
  pinMode(touchSensorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(builtinLedPin, OUTPUT);
  pinMode(secondLedPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Relay is off at start
  digitalWrite(builtinLedPin, HIGH); // Built-in LED is off at start
  digitalWrite(secondLedPin, HIGH); // Second LED is off at start
}

void loop() {
  bool currentState = digitalRead(touchSensorPin);

  if (lastState == LOW && currentState == HIGH) {
    // The touch sensor has just been touched
    touchStartTime = millis();

    if (touchStartTime - lastTouchTime < debounceDelay) {
      // The sensor has been double tapped
      tapCount++;
    } else {
      tapCount = 1; // reset tap count for new sequence
    }

    lastTouchTime = touchStartTime;
  } else if (lastState == HIGH && currentState == LOW) {
    // The touch sensor has just been released
    if (millis() - touchStartTime >= touchInterruptTime) {
      // The sensor was touched for 2 seconds or longer
      digitalWrite(relayPin, HIGH); // turn relay OFF
      digitalWrite(builtinLedPin, HIGH); // turn built-in LED OFF
      digitalWrite(secondLedPin, LOW); // turn second LED ON to signal cancellation
      //delay(1000); // wait for a second for the user to notice the cancellation LED
      
      currentMode = OFF;
      tapCount = 0; // reset tap count
    }
  }

  if (tapCount == 1 && millis() - lastTouchTime >= doubleTapInterval) {
    // It's a single tap, as no second tap occurred within the doubleTapInterval
    currentMode = FEW_SECONDS;
    digitalWrite(relayPin, LOW); // turn relay ON
    digitalWrite(builtinLedPin, HIGH);
    digitalWrite(secondLedPin, HIGH); // turn second LED OFF to signal cancellation 
    tapCount = 0; // reset tap count
    
  } else if (tapCount == 2) {
    // It's a double tap
    currentMode = FEW_MINUTES;
    digitalWrite(relayPin, LOW); // turn relay ON
    digitalWrite(builtinLedPin, LOW); // turn built-in LED ON to signal one-minute mode
    digitalWrite(secondLedPin, HIGH); // turn second LED OFF to signal cancellation
    tapCount = 0; // reset tap count
  }

  if (currentMode == FEW_SECONDS && millis() - lastTouchTime >= fewSeconds) {
    // FEW seconds have passed since the single tap
    digitalWrite(relayPin, HIGH); // turn relay OFF
    digitalWrite(builtinLedPin, HIGH); // turn built-in LED OFF
    digitalWrite(secondLedPin, HIGH); // turn second LED OFF after cancellation signal
      
    
    currentMode = OFF;
  } else if (currentMode == FEW_MINUTES && millis() - lastTouchTime >= fewMinutes) {
    // FEW minute has passed since the double tap
    digitalWrite(relayPin, HIGH); // turn relay OFF
    digitalWrite(builtinLedPin, HIGH); // turn built-in LED OFF
    digitalWrite(secondLedPin, HIGH); // turn second LED OFF after cancellation signal
      
    
    currentMode = OFF;
  }

  lastState = currentState;
}
