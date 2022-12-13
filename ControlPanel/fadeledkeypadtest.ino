/* @file EventSerialKeypad.pde
 || @version 1.0
 || @author Alexander Brevig
 || @contact alexanderbrevig@gmail.com
 ||
 || @description
 || | Demonstrates using the KeypadEvent.
 || #
 */
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
    {'1','2','3', 'A'},
    {'4','5','6', 'B'},
    {'7','8','9', 'C'},
    {'*','0','#', 'D'}
};

byte colPins[COLS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

KeyState lastState = IDLE;

#define LED 11
bool ledOn = false;
int brightness = 0;
bool turnUp = true;

bool isHold = false;

int previousTime = 0;
int interval = 50;

void setup(){
    Serial.begin(9600);
    keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
    pinMode(LED, OUTPUT);
}

void loop(){
  char key = keypad.getKey();

  if (isHold) {
    int currentTime = millis();

    if (currentTime - previousTime >= interval) {
      previousTime = currentTime;

      fadeLED(LED);
    }
  }

}

void fadeLED(int ledpin) {
  if (brightness >= 250) {
    turnUp == false;
    brightness = 250;
  } else if (brightness <= 5) {
    turnUp == true;
    brightness = 5;
  }
  if (turnUp) {
    brightness += 5;
    ledOn = true;
    Serial.println(brightness);
    analogWrite(ledpin, brightness);
  } else if (!turnUp) {
    brightness -= 5;
    Serial.println(brightness);
    analogWrite(ledpin, brightness);
    if (brightness == 0) {
      ledOn = false;
    }
  }
}

// Taking care of some special events.
void keypadEvent(KeypadEvent key){
    switch (keypad.getState()){
    case PRESSED:
        if (key == '0') {
          lastState = PRESSED;
        }
        break;

    case RELEASED:
        if (key == '0') {
            if (lastState == PRESSED) {
              if (ledOn) {
                brightness = 0;
                analogWrite(LED, brightness);
                ledOn = !ledOn;
                turnUp = true;
              } else {
                brightness = 255;
                analogWrite(LED, brightness);
                ledOn = !ledOn;
                turnUp = false;
              }
            } else if (lastState == HOLD) {
              Serial.println("HOLD");
              turnUp = !turnUp;
              isHold = false;
            }
        }
        break;

    case HOLD:
        if (key == '0') {
            lastState = HOLD;

            

            isHold = true;
        }
        break;
    }
}
