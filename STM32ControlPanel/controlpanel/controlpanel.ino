﻿// Include the libraries
#include <TM1637Display.h>
#include <Keypad.h>

// Define the connections pins
// Alarm display pins
#define CLK0 PB6
#define DIO0 PB7
// Temp display pins
#define CLK1 PB8
#define DIO1 PB9

// 7-segment display
TM1637Display display0 = TM1637Display(CLK0, DIO0); // init display
TM1637Display display1 = TM1637Display(CLK1, DIO1); // init display
const uint8_t allOff[] = {SEG_G, SEG_G, SEG_G, SEG_G};

// Curtain alarm
bool alarmIsOn = false; 

int hour = 23;
int minute = 30;

typedef enum {PLUS, MINUS, PLUSFAST, MINUSFAST, NONE} alarmChangeState;
alarmChangeState changeState = NONE;

int previousTime = 0;
int interval = 150;

// temp
int temp = 20;

const uint8_t ROWS = 4;
const uint8_t COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '+', '0', '-', 'D' }
};

uint8_t colPins[COLS] = { PA3, PA2, PA1, PA0 }; // Pins connected to C1, C2, C3, C4
uint8_t rowPins[ROWS] = { PA7, PA6, PA5, PA4 }; // Pins connected to R1, R2, R3, R4

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


void alarmRollover() {
  // Increase hour when minute reaches 60
  if (minute >= 60) {
    minute -= 60;
    hour += 1;
  }
  // Decrease hour when minute reaches below 0
  if (minute < 0) {
    minute += 60;
    hour -= 1;
  }

  // wraparound
  hour = (hour > 23) ? 0 : hour;
  hour = (hour < 0) ? 23 : hour;
}

void changingTime() {
  switch (changeState) {
    case PLUS:
      minute += 5;
      break;
    case MINUS:
      minute -= 5;
      break;
    case PLUSFAST:
      minute += 15;
      break;
    case MINUSFAST:
      minute -= 15;
      break;
    case NONE:
      break;
  }
}

void changeTemp(char key) {
  if (key == '1') {
    temp += 1;
  }
  if (key == '2') {
    temp -= 1;
  }
}

void setup() {
  Serial.begin(9600);
	// Set the display brightness (0-7)
	display0.setBrightness(5);
  display1.setBrightness(5);
	
	// Clear the display
	display0.clear();
  display1.clear();

  keypad.setHoldTime(1000);
  keypad.addEventListener(keypadEvent);
}

void loop() {	
  char key = keypad.getKey();
  
  // clear screen
  if (key == '3') {
    display0.clear();
    display1.clear();
  }

  // handling the time of clock
  int currentTime = millis();
  if (currentTime - previousTime >= interval) {
    previousTime = currentTime;

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
    display0.setSegments(allOff);
  }

  // Display temp
  display1.showNumberDec(temp, false, 2, 1);
}

// Taking care of some special events.
void keypadEvent(KeypadEvent key){
    switch (keypad.getState()){
    case PRESSED:
      // clock display
      if (key == '+') {
          changeState = PLUS;
      }
      if (key == '-') {
          changeState = MINUS;
      }
      if (key == '0') {
        alarmIsOn = !alarmIsOn;
      }

      // temp display
      if (key == '1') {
        temp += 1;
      }
      if (key == '2') {
        temp -= 1;
      }
      break;

    case RELEASED:
      if (key == '+' || key == '-') {
        changeState = NONE;
        interval = 150;
      }
      break;

    case HOLD:
      if (key == '+') {
        changeState = PLUSFAST;
        interval = 100;
      }
      if (key == '-') {
        changeState = MINUSFAST;
        interval = 100;
      }
      break;
    }
}