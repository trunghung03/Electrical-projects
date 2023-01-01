// This code truly could use some OOP paradigm, but due to time limitation.

#include <Key.h>
#include <Keypad.h>
#include "led.h"
#include "segments.h"

// Display
const uint8_t ROWS = 4;
const uint8_t COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '+', '0', '-', 'D' }
};

uint8_t colPins[COLS] = { 46, 48, 50, 52 }; // Pins connected to C1, C2, C3, C4
uint8_t rowPins[ROWS] = { 38, 40, 42, 44 }; // Pins connected to R1, R2, R3, R4

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LED and LED switches pins
#define LED1PIN 9
#define LED1SWPIN 28

#define LED2PIN 10
#define LED2SWPIN 30

#define LED3PIN 11
#define LED3SWPIN 32

#define LED4PIN 12
#define LED4SWPIN 34

#define LED5PIN 13
#define LED5SWPIN 36

Led LED1(LED1PIN, LED1SWPIN);
Led LED2(LED2PIN, LED2SWPIN);
Led LED3(LED3PIN, LED3SWPIN);
Led LED4(LED4PIN, LED4SWPIN);
Led LED5(LED5PIN, LED5SWPIN);

// Timer for fade
int previousTime = 0;
int FadeInterval = 50;

void setup() {
    Serial.begin(9600);
    keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
    keypad.setHoldTime(1000);
    
    pinMode(LED1.m_PIN, OUTPUT);
    pinMode(LED2.m_PIN, OUTPUT);
    pinMode(LED3.m_PIN, OUTPUT);
    pinMode(LED4.m_PIN, OUTPUT);
    pinMode(LED5.m_PIN, OUTPUT);

    pinMode(LED1.m_swPIN, INPUT_PULLUP);
    pinMode(LED2.m_swPIN, INPUT_PULLUP);
    pinMode(LED3.m_swPIN, INPUT_PULLUP);
    pinMode(LED4.m_swPIN, INPUT_PULLUP);
    pinMode(LED5.m_swPIN, INPUT_PULLUP);

    // Set the display brightness (0-7)
    display0.setBrightness(5);
    display1.setBrightness(5);
    
    // Clear the display
    display0.clear();
    display1.clear();
}

void loop() {
    char key = keypad.getKey();

    if (key != NO_KEY)
        Serial.println(key);

    if (LED1.m_isHold) {
        handle_hold(LED1);
    }
    else if (LED2.m_isHold) {
        handle_hold(LED2);
    }
    else if (LED3.m_isHold) {
        handle_hold(LED3);
    }
    else if (LED4.m_isHold) {
        handle_hold(LED4);
    }
    else if (LED5.m_isHold) {
        handle_hold(LED5);
    }

    ledsw(LED1);
    ledsw(LED2);
    ledsw(LED3);
    ledsw(LED4);
    ledsw(LED5);

    // clear screen
    if (key == '3') {
      display0.clear();
      display1.clear();
    }
  
    // handling the time of clock
    int currentTime = millis();
    if (currentTime - previousTimeDis >= interval) {
      previousTimeDis = currentTime;
  
      changingTime();
      alarmRollover();
    }
    
    // Create time format to display
    int displaytime = (hour * 100) + minute;
  
    // Display alarm
    if (alarmIsOn) {
      // Display the current time in 24 hour format with leading zeros and a center colon enabled
      display0.showNumberDecEx(displaytime, 0b11100000, true);
    } else {
      display0.setSegments(allOffAlarm);
    }
  
    // Display AC status
    if (acIsOn) {
      display1.showNumberDec(temp, false, 2, 1);
    } else {
      display1.setSegments(allOffAC);
    }
}

void handle_hold(Led &led) {
    int currentTime = millis();
    if (currentTime - previousTime >= FadeInterval) {
        previousTime = currentTime;
        fadeLED(led);
    }
}

void ledsw(Led &led) {
    int reading = digitalRead(led.m_swPIN);

    if (reading != led.swFlag) {
        //Serial.println("here");
        if (led.m_ledOn) {
            switchLed(led, false);
        }
        else {
            switchLed(led, true);
        }
    }
    led.swFlag = reading;
}

void fadeLED(Led &led) {
    if (led.m_brightness >= 250) {
        led.m_turnUp == false;
        led.m_brightness = 250;
    }
    else if (led.m_brightness <= 5) {
        led.m_turnUp == true;
        led.m_brightness = 5;
    }
    if (led.m_turnUp) {
        led.m_brightness += 5;
        led.m_ledOn = true;
        analogWrite(led.m_PIN, led.m_brightness);
    }
    else if (!(led.m_turnUp)) {
        led.m_brightness -= 5;
        analogWrite(led.m_PIN, led.m_brightness);
        if (led.m_brightness == 0) {
            led.m_ledOn = false;
        }
    }
}

void switchLed(Led &led, bool turnOn) {
    if (turnOn) {
        led.m_brightness = 255;
        analogWrite(led.m_PIN, led.m_brightness);
        led.m_turnUp = false;
        led.m_ledOn = true;
    }
    else {
        led.m_brightness = 0;
        analogWrite(led.m_PIN, led.m_brightness);
        led.m_turnUp = true;
        led.m_ledOn = false;
    }
}

void alternating1() {
    switchLed(LED1, true);
    switchLed(LED2, false);
    switchLed(LED3, true);
    switchLed(LED4, false);
    switchLed(LED5, true);
}

void alternating2() {
    switchLed(LED1, false);
    switchLed(LED2, true);
    switchLed(LED3, false);
    switchLed(LED4, true);
    switchLed(LED5, false);
}

void allOff() {
    switchLed(LED1, false);
    switchLed(LED2, false);
    switchLed(LED3, false);
    switchLed(LED4, false);
    switchLed(LED5, false);
}

void allOn() {
    switchLed(LED1, true);
    switchLed(LED2, true);
    switchLed(LED3, true);
    switchLed(LED4, true);
    switchLed(LED5, true);
}

void handle_hold_start(Led &led) {
    led.m_lastState = HOLD;
    led.m_isHold = true;
}

void handle_hold_end(Led &led) {
    led.m_turnUp = !led.m_turnUp;
    led.m_isHold = false;
}

void handle_release(Led &led) {
    if (led.m_lastState == PRESSED) {
        if (led.m_ledOn) {
            switchLed(led, false);
        }
        else {
            switchLed(led, true);
        }
    }
    else if (led.m_lastState == HOLD) {
        handle_hold_end(led);
    }
}

// Taking care of some special events.
void keypadEvent(KeypadEvent key) {
    switch (keypad.getState()) {
    case PRESSED:
        switch (key) {
        case '1':
            LED1.m_lastState = PRESSED;
            break;
        case '2':
            LED2.m_lastState = PRESSED;
            break;
        case '3':
            LED3.m_lastState = PRESSED;
            break;
        case '4':
            LED4.m_lastState = PRESSED;
            break;
        case '5':
            LED5.m_lastState = PRESSED;
            break;
        case '6':
            alternating1();
            break;
        case '7':
            alternating2();
            break;
        case '8':
            allOff();
            break;
        case '9':
            allOn();
            break;
        }

        // clock display
        if (key == '+' && alarmIsOn) {
            changeState = PLUS;
        }
        if (key == '-' && alarmIsOn) {
            changeState = MINUS;
        }
        if (key == '0') {
          alarmIsOn = !alarmIsOn;
        }
  
        // temp display
        if (key == 'A' && acIsOn) {
          temp += 1;
        }
        if (key == 'B' && acIsOn) {
          temp -= 1;
        }
        if (key == 'C') {
          acIsOn = !acIsOn;
        }
                
        break;

    case RELEASED:
        switch (key) {
        case '1':
            handle_release(LED1);
            break;
        case '2':
            handle_release(LED2);
            break;
        case '3':
            handle_release(LED3);
            break;
        case '4':
            handle_release(LED4);
            break;
        case '5':
            handle_release(LED5);
            break;
        }

        if (key == '+' || key == '-') {
            changeState = NONE;
            interval = 150;
        }
        
        break;

    case HOLD:
        switch (key) {
        case '1':
            handle_hold_start(LED1);
            break;
        case '2':
            handle_hold_start(LED2);
            break;
        case '3':
            handle_hold_start(LED3);
            break;
        case '4':
            handle_hold_start(LED4);
            break;
        case '5':
            handle_hold_start(LED5);
            break;
        case '+':
            changeState = PLUSFAST;
            interval = 100;
            break;
        case '-':
            changeState = MINUSFAST;
            interval = 100;
            break;
        }

        break;
    }
}
