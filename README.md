# TouchPumpV1
This project allows you to control a relay (and hence a pump) using a TTP223 touch sensor and a ESP8266 microcontroller (NodeMCU). The system has three modes:

1.  Single tap on the touch sensor runs the pump for a certain short duration.
2.  Double tap on the touch sensor runs the pump for a longer duration.
3.  Holding the touch sensor for 2 seconds interrupts the operation and stops the pump immediately.

There are also two built-in LED indicators on the ESP8266 which indicate when the pump is running for the long duration and when the pump operation is interrupted.

1.  Materials Needed:

    -   ESP8266 (NodeMCU v3 or similar would work)
    -   TTP223 Touch Sensor Module
    -   Relay Module (5V, Single Channel or according to your pump's requirement)
    -   Pump (Your existing pump)
    -   Jumper Wires
    -   Breadboard (optional, for easy wiring)
2.  Connections:

    Here's how you should connect the components:

    -   ESP8266 VIN to Relay VCC
    -   ESP8266 GND to Relay GND
    -   ESP8266 D1 to Relay IN1
    -   ESP8266 3V3 to TTP223 VCC
    -   ESP8266 GND to TTP223 GND
    -   ESP8266 D2 to TTP223 SIG
    -   Connect Pump to the relay's NO (Normally Open) and Common (COM) terminals. The exact connection will depend on your pump's specifications and the power source you are using.

    *Note: Be very careful while connecting your pump to the relay. Do not connect it directly to your ESP8266 as it can't handle high currents.*

3.  Code:

    You'll need to program your ESP8266. You can do this using the Arduino IDE. Use the provided Arduino sketch. Adjust the time durations as needed. The `fewSeconds` and `fewMinutes` variables represent the short and long durations for the pump to run, respectively.

    `ESP8266_Pump_Touch*.ino`

    This code will read the state of the touch sensor. If the touch sensor is touched (i.e., the sensor state is `HIGH`), the relay (and thus the pump) will be turned on. If the touch sensor is not being touched (i.e., the sensor state is `LOW`), the relay will be turned off.

5.  Upload the Code to ESP8266:

    -   Download and install the Arduino IDE from the official Arduino website.
    -   Open the Arduino IDE and go to File > New. This will open a new script.
    -   Copy the above code into the script.
    -   Connect your ESP8266 to your computer using a USB cable.
    -   Go to Tools > Board and select "NodeMCU 1.0 (ESP-12E Module)" or your respective board.
    -   Go to Tools > Port and select the COM port that your ESP8266 is connected to.
    -   Click on the Upload button (the right arrow in the top left corner of the IDE).
6.  Operation:

    -   Single tap on the touch sensor will run the pump for a short duration.
    -   Double tap on the touch sensor will run the pump for a longer duration, and the built-in LED indicator will be ON.
    -   Holding the touch sensor for 2 seconds will immediately stop the pump, and the second LED will light up.
  
7.  Notes:
    -  Remember to always be very careful when working with electricity and never touch any of the live connections.
  
8. Thank you:
   -   Thanks to OpenAI for helping out.


