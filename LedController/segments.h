// Include the libraries
#include <TM1637Display.h>

// Define the connections pins
// Alarm display pins
#define CLK0 41
#define DIO0 43
// Temp display pins
#define CLK1 45
#define DIO1 47

// 7-segment display
TM1637Display display0 = TM1637Display(CLK0, DIO0); // init display
TM1637Display display1 = TM1637Display(CLK1, DIO1); // init display
const uint8_t allOffAlarm[] = {SEG_G, SEG_G, SEG_G, SEG_G};
const uint8_t allOffAC[] = {0, SEG_G, SEG_G, 0};

// Curtain alarm
bool alarmIsOn = false; 

int hour = 23;
int minute = 30;

typedef enum {PLUS, MINUS, PLUSFAST, MINUSFAST, NONE} alarmChangeState;
alarmChangeState changeState = NONE;

int previousTimeDis = 0;
int interval = 150;

// temp
int temp = 20;
bool acIsOn = false;

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