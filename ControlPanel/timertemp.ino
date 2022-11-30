// Include the libraries
#include <TM1637Display.h>
#include <Keypad.h>

// Define the connections pins
#define CLK 0
#define DIO 1

TM1637Display display = TM1637Display(CLK, DIO); // init clock
const uint8_t allOff[] = {SEG_G, SEG_G, SEG_G, SEG_G};

bool alarmIsOn = false;
bool toggleTime = false; // if false then change hour, true change minute

int hour = 10;
int minute = 10;

// Keypad thing
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'+','0','-','D'}
};
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte rowPins[ROWS] = {13, 12, 11, 10}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void clock(char key) {
  if (key == '0') {
    alarmIsOn = !alarmIsOn;
  }

  if (key == 'C') {
      hour = 0;
      minute = 0;
  }

  if (alarmIsOn) {
    if (key == 'D') {
      toggleTime = !toggleTime;
    }

    if (key == '+') {
      if (!toggleTime) {
          hour += 1;
      } else {
          minute += 5;
      }
    }

    if (key == '-') {
      if (!toggleTime) {
          hour -= 1;
      } else {
          minute -= 5;
      }
    }
  }

  // Boundchecking
  hour = (hour > 24) ? 24 : hour;
  hour = (hour < 0) ? 0 : hour;

  minute = (minute > 60) ? 60 : minute;
  minute = (minute < 0) ? 0 : minute;

}

void setup() {
	// Set the display brightness (0-7)
	display.setBrightness(5);
	
	// Clear the display
	display.clear();
}

void loop() {
	// Get current date and time
	
  char key = keypad.getKey();
  clock(key);

  
	// Create time format to display
	int displaytime = (hour * 100) + minute;


  if (alarmIsOn) {
	  // Display the current time in 24 hour format with leading zeros and a center colon enabled
	  display.showNumberDecEx(displaytime, 0b11100000, true);
  } else {
    display.setSegments(allOff);
  }
}
