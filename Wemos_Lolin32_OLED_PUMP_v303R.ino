#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int touchSensorPin = 12; // The pin where touch sensor is connected
const int relayPin = 14; // The pin where relay is connected
//const int builtinLedPin = LED_BUILTIN; // Built-in LED on ESP32
//const int secondLedPin = 0; // Second LED pin, please modify according to your wiring

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
  //pinMode(builtinLedPin, OUTPUT);
  
  digitalWrite(relayPin, HIGH); // Relay is off at start
  //digitalWrite(builtinLedPin, LOW); // Built-in LED is off at start
  
  
  Wire.begin(5, 4);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Touch Pump v32.303");
  display.display();

  // Start serial communication for debugging
  Serial.begin(9600);
  Serial.println("Setup complete");
  delay(3000);

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
      //digitalWrite(builtinLedPin, LOW); // turn built-in LED OFF
      
      //delay(1000); // wait for a second for the user to notice the cancellation LED
      
      currentMode = OFF;
      tapCount = 0; // reset tap count
    }
  }

  if (tapCount == 1 && millis() - lastTouchTime >= doubleTapInterval) {
    // It's a single tap, as no second tap occurred within the doubleTapInterval
    currentMode = FEW_SECONDS;
    digitalWrite(relayPin, LOW); // turn relay ON
    //digitalWrite(builtinLedPin, LOW);
     
    tapCount = 0; // reset tap count
    
  } else if (tapCount == 2) {
    // It's a double tap
    currentMode = FEW_MINUTES;
    digitalWrite(relayPin, LOW); // turn relay ON
    //digitalWrite(builtinLedPin, LOW); // turn built-in LED ON to signal one-minute mode
    
    tapCount = 0; // reset tap count
  }

  if (currentMode == FEW_SECONDS && millis() - lastTouchTime >= fewSeconds) {
    // FEW seconds have passed since the single tap
    digitalWrite(relayPin, HIGH); // turn relay OFF
    //digitalWrite(builtinLedPin, LOW); // turn built-in LED OFF
    
      
    
    currentMode = OFF;
  } else if (currentMode == FEW_MINUTES && millis() - lastTouchTime >= fewMinutes) {
    // FEW minute has passed since the double tap
    digitalWrite(relayPin, HIGH); // turn relay OFF
    //digitalWrite(builtinLedPin, LOW); // turn built-in LED OFF
    
      
    
    currentMode = OFF;
  }

  lastState = currentState;

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);

  switch(currentMode) {
    case OFF:
      display.setTextSize(1);
      display.println(F("System Status: OFF"));
      display.println(F(""));
      
      display.print(F("Single Tap: "));
      display.println(fewSeconds/1000);
      display.println(F("Run for few seconds"));
      display.print(F("Double Tap: "));
      display.println(fewMinutes/1000/60);
      display.println(F("Run for few minutes"));
      display.println(F("STOP: "));
      display.println(F("Hold for 2 seconds"));
      break;
    case FEW_SECONDS:
      display.println(F("System Status:Running"));
      display.println(F(""));
      display.println(F("MODE: FEW seconds"));
      display.setTextSize(2);
      display.print(fewSeconds/1000);
      display.println(F("  sec"));
      display.setTextSize(3);
      display.print((fewSeconds - (millis() - lastTouchTime))/1000);
      break;
    case FEW_MINUTES:
      display.println(F("System Status:Running"));
      display.println(F(""));
      display.println(F("MODE: FEW minutes"));
      display.setTextSize(2);
      display.print(fewMinutes/1000/60);
      display.println(F("   min"));
      display.setTextSize(3);
      display.print((fewMinutes - (millis() - lastTouchTime))/1000);
      break;
  }

  display.display();
  
}
