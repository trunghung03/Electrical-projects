// This code truly could use some OOP paradigm, but due to time limitation.

#include <Key.h>
#include <Keypad.h>
#include "led.h"

// Display
const uint8_t ROWS = 3;
const uint8_t COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' }
};

uint8_t colPins[COLS] = { 8, 11, 12, 13 }; // Pins connected to C1, C2, C3, C4
uint8_t rowPins[ROWS] = { 2, 4, 7 }; // Pins connected to R1, R2, R3, R4

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LED and LED switches pins
#define LED1PIN 3
#define LED1SWPIN A0

#define LED2PIN 5
#define LED2SWPIN A1

#define LED3PIN 6
#define LED3SWPIN A2

#define LED4PIN 9
#define LED4SWPIN A3

#define LED5PIN 10
#define LED5SWPIN A4

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
        }

        break;
    }
}
