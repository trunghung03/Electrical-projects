// This code truly could use some OOP paradigm, but due to time limitation.

#include <Keypad.h>

const uint8_t ROWS = 4;
const uint8_t COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

uint8_t colPins[COLS] = { A1, A0, 12, 13 }; // Pins connected to C1, C2, C3, C4
uint8_t rowPins[ROWS] = { A5, A4, A3, A2 }; // Pins connected to R1, R2, R3, R4

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

#define LED1 5
bool led1On = false;
int brightness1 = 0;
bool turn1Up = true;
KeyState lastState1 = IDLE;
bool isHold1 = false;

#define LED2 6
bool led2On = false;
int brightness2 = 0;
bool turn2Up = true;
KeyState lastState2 = IDLE;
bool isHold2 = false;

#define LED3 9
bool led3On = false;
int brightness3 = 0;
bool turn3Up = true;
KeyState lastState3 = IDLE;
bool isHold3 = false;

#define LED4 10
bool led4On = false;
int brightness4 = 0;
bool turn4Up = true;
KeyState lastState4 = IDLE;
bool isHold4 = false;

#define LED5 11
bool led5On = false;
int brightness5 = 0;
bool turn5Up = true;
KeyState lastState5 = IDLE;
bool isHold5 = false;


#define LED1SW 2
int led1Flag = LOW;

#define LED2SW 3
int led2Flag = LOW;

#define LED3SW 4
int led3Flag = LOW;

#define LED4SW 7
int led4Flag = LOW;

#define LED5SW 8
int led5Flag = LOW;

int previousTime = 0;
int interval = 50;

void setup(){
    Serial.begin(9600);
    keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(LED5, OUTPUT);

    pinMode(LED1SW, INPUT_PULLUP);
    pinMode(LED2SW, INPUT_PULLUP);
    pinMode(LED3SW, INPUT_PULLUP);
    pinMode(LED4SW, INPUT_PULLUP);
    pinMode(LED5SW, INPUT_PULLUP);
}

void loop(){
  char key = keypad.getKey();

  if (isHold1) {
    int currentTime = millis();
    if (currentTime - previousTime >= interval) {
      previousTime = currentTime;
      fadeLED(LED1, &led1On, &brightness1, &turn1Up);
    }
  } else if (isHold2) {
    int currentTime = millis();
    if (currentTime - previousTime >= interval) {
      previousTime = currentTime;
      fadeLED(LED2, &led2On, &brightness2, &turn2Up);
    }
  } else if (isHold3) {
    int currentTime = millis();
    if (currentTime - previousTime >= interval) {
      previousTime = currentTime;
      fadeLED(LED3, &led3On, &brightness3, &turn3Up);
    }
  } else if (isHold4) {
    int currentTime = millis();
    if (currentTime - previousTime >= interval) {
      previousTime = currentTime;
      fadeLED(LED4, &led4On, &brightness4, &turn4Up);
    }
  } else if (isHold5) {
    int currentTime = millis();
    if (currentTime - previousTime >= interval) {
      previousTime = currentTime;
      fadeLED(LED5, &led5On, &brightness5, &turn5Up);
    }
  }

  ledsw(LED1SW, LED1, &led1Flag, &led1On, &brightness1, &turn1Up);
  ledsw(LED2SW, LED2, &led2Flag, &led2On, &brightness2, &turn2Up);
  ledsw(LED3SW, LED3, &led3Flag, &led3On, &brightness3, &turn3Up);
  ledsw(LED4SW, LED4, &led4Flag, &led4On, &brightness4, &turn4Up);
  ledsw(LED5SW, LED5, &led5Flag, &led5On, &brightness5, &turn5Up);
}

void ledsw(int ledsw, int ledpin, int *flag, bool *isOn, int *brightness, bool *turnUp) {
  int led1reading = digitalRead(ledsw);

  if (led1reading != *flag) {
    if (*isOn) {
      //Serial.println("here");
      *brightness = 0;
      analogWrite(ledpin, *brightness);
      *turnUp = true;
    } else {
      //Serial.println("here");
      *brightness = 255;
      analogWrite(ledpin, *brightness);
      *turnUp = false;
    }
    *isOn = !(*isOn);
  }
  *flag = led1reading;
}

void fadeLED(int ledpin, bool *isOn, int *brightness, bool *turnUp) {
  if (*brightness >= 250) {
    *turnUp == false;
    *brightness = 250;
  } else if (*brightness <= 5) {
    *turnUp == true;
    *brightness = 5;
  }
  if (*turnUp) {
    *brightness += 5;
    *isOn = true;
    //Serial.println(*brightness);
    analogWrite(ledpin, *brightness);
  } else if (!(*turnUp)) {
    *brightness -= 5;
    //Serial.println(*brightness);
    analogWrite(ledpin, *brightness);
    if (*brightness == 0) {
      *isOn = false;
    }
  }
}

void alternating1() {
  brightness1 = 255;
  analogWrite(LED1, brightness1);
  turn1Up = false;
  led1On = true;

  brightness2 = 0;
  analogWrite(LED2, brightness2);
  turn2Up = true;
  led2On = false;

  brightness3 = 255;
  analogWrite(LED3, brightness3);
  turn3Up = false;
  led3On = true;

  brightness4 = 0;
  analogWrite(LED4, brightness4);
  turn4Up = true;
  led4On = false;

  brightness5 = 255;
  analogWrite(LED5, brightness5);
  turn5Up = false;
  led5On = true;
}

void alternating2() {
  brightness1 = 0;
  analogWrite(LED1, brightness1);
  turn1Up = true;
  led1On = false;

  brightness2 = 255;
  analogWrite(LED2, brightness2);
  turn2Up = false;
  led2On = true;

  brightness3 = 0;
  analogWrite(LED3, brightness3);
  turn3Up = true;
  led3On = false;

  brightness4 = 255;
  analogWrite(LED4, brightness4);
  turn4Up = false;
  led4On = true;

  brightness5 = 0;
  analogWrite(LED5, brightness5);
  turn5Up = true;
  led5On = false;
}

void allOff() {
    brightness1 = 0;
  analogWrite(LED1, brightness1);
  turn1Up = true;
  led1On = false;

  brightness2 = 0;
  analogWrite(LED2, brightness2);
  turn2Up = true;
  led2On = false;

  brightness3 = 0;
  analogWrite(LED3, brightness3);
  turn3Up = true;
  led3On = false;

  brightness4 = 0;
  analogWrite(LED4, brightness4);
  turn4Up = true;
  led4On = false;

  brightness5 = 0;
  analogWrite(LED5, brightness5);
  turn5Up = true;
  led5On = false;
}

void allOn() {
  brightness1 = 255;
  analogWrite(LED1, brightness1);
  turn1Up = false;
  led1On = true;

  brightness2 = 255;
  analogWrite(LED2, brightness2);
  turn2Up = false;
  led2On = true;

  brightness3 = 255;
  analogWrite(LED3, brightness3);
  turn3Up = false;
  led3On = true;

  brightness4 = 255;
  analogWrite(LED4, brightness4);
  turn4Up = false;
  led4On = true;

  brightness5 = 255;
  analogWrite(LED5, brightness5);
  turn5Up = false;
  led5On = true;
}

// Taking care of some special events.
void keypadEvent(KeypadEvent key){
    switch (keypad.getState()) {
    case PRESSED:
        if (key == '1') {
          lastState1 = PRESSED;
        } else if (key == '2') {
          lastState2 = PRESSED;
        } else if (key == '3') {
          lastState3 = PRESSED;
        } else if (key == '4') {
          lastState4 = PRESSED;
        } else if (key == '5') {
          lastState5 = PRESSED;
        } else if (key == '6') {
          alternating1();
        } else if (key == '7') {
          alternating2();
        } else if (key == '8') {
          allOff();
        } else if (key == '9') {
          allOn();
        }
        break;

    case RELEASED:
        if (key == '1') {
            if (lastState1 == PRESSED) {
              if (led1On) {
                brightness1 = 0;
                analogWrite(LED1, brightness1);
                turn1Up = true;
              } else {
                brightness1 = 255;
                analogWrite(LED1, brightness1);
                turn1Up = false;
              }
              led1On = !led1On;
            } else if (lastState1 == HOLD) {
              //Serial.println("HOLD");
              turn1Up = !turn1Up;
              isHold1 = false;
            }
        } else if (key == '2') {
            if (lastState2 == PRESSED) {
              if (led2On) {
                brightness2 = 0;
                analogWrite(LED2, brightness2);
                turn2Up = true;
              } else {
                brightness2 = 255;
                analogWrite(LED2, brightness2);
                turn2Up = false;
              }
              led2On = !led2On;
            } else if (lastState2 == HOLD) {
              //Serial.println("HOLD");
              turn2Up = !turn2Up;
              isHold2 = false;
            }
        } else if (key == '3') {
            if (lastState3 == PRESSED) {
              if (led3On) {
                brightness3 = 0;
                analogWrite(LED3, brightness3);
                turn3Up = true;
              } else {
                brightness3 = 255;
                analogWrite(LED3, brightness3);
                turn3Up = false;
              }
              led3On = !led3On;
            } else if (lastState3 == HOLD) {
              //Serial.println("HOLD");
              turn3Up = !turn3Up;
              isHold3 = false;
            }
        } else if (key == '4') {
            if (lastState4 == PRESSED) {
              if (led4On) {
                brightness4 = 0;
                analogWrite(LED4, brightness4);
                turn4Up = true;
              } else {
                brightness4 = 255;
                analogWrite(LED4, brightness4);
                turn4Up = false;
              }
              led4On = !led4On;
            } else if (lastState4 == HOLD) {
              //Serial.println("HOLD");
              turn4Up = !turn4Up;
              isHold4 = false;
            }
        } else if (key == '5') {
            if (lastState5 == PRESSED) {
              if (led5On) {
                brightness5 = 0;
                analogWrite(LED5, brightness5);
                turn5Up = true;
              } else {
                brightness5 = 255;
                analogWrite(LED5, brightness5);
                turn5Up = false;
              }
              led5On = !led5On;
            } else if (lastState5 == HOLD) {
              //Serial.println("HOLD");
              turn5Up = !turn5Up;
              isHold5 = false;
            }
        }
        break;

    case HOLD:
        if (key == '1') {
            lastState1 = HOLD;
            isHold1 = true;
        } else if (key == '2') {
            lastState2 = HOLD;
            isHold2 = true;
        } else if (key == '3') {
            lastState3 = HOLD;
            isHold3 = true;
        } else if (key == '4') {
            lastState4 = HOLD;
            isHold4 = true;
        } else if (key == '5') {
            lastState5 = HOLD;
            isHold5 = true;
        }
        break;
    }
}
