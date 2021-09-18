// Simple gamepad example that demonstraits how to read five Arduino
// digital pins and map them to the Arduino Joystick library.
//
// The digital pins 2 - 8 are grounded when they are pressed.
//
// Pin 2 = LEFT
// Pin 4 = RIGHT
// Pin 7 = B1
// Pin 8 = B2
// Pin 15 = B3
// Pin 14 = B4
// Pin 16 = B5
//
// NOTE: This sketch file is for use with Arduino Leonardo and
//       Arduino Micro only.
//--------------------------------------------------------------------

#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
                   7, 0,                  // Button Count, Hat Switch Count
                   false, false, false,    // have X Axis, no Y Axis, no Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering

int xR = 0;
int xL = 0;

void setup() {
  // Initialize Button Pins
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);

  // Initialize Joystick Library
  Joystick.begin();
  Joystick.setXAxisRange(-100, 100);

}

// Last state of the buttons
int lastButtonState[7] = {0, 0, 0, 0, 0, 0, 0};

void loop() {

  // Read pin values
  for (int index = 0; index < 7; index++)
  {
    int currentButtonState = !digitalRead(index + 2);
    if (currentButtonState != lastButtonState[index])
    {
      switch (index)
      {
        case 0: // B1
          Joystick.setButton(0, currentButtonState);
          break;
        case 1: // B2
          Joystick.setButton(1, currentButtonState);
          break;
        case 2: // B3
          Joystick.setButton(2, currentButtonState);
          break;
        case 3: // B4
          Joystick.setButton(3, currentButtonState);
          break;
        case 4: // B5
          Joystick.setButton(4, currentButtonState);
          break;
        case 5: // B6
          Joystick.setButton(5, currentButtonState);
          break;
        case 6: // B7
          Joystick.setButton(6, currentButtonState);
          break;
      }
      lastButtonState[index] = currentButtonState;
    }
  }

  delay(10);
}
