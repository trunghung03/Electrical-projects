#include <U8glib.h>          // OLED Display library
#include <RTClib.h>
#include <Keypad.h>

// Format OLED display
#define SLOT_WIDTH 20
#define Y_OFFSET 32

RTC_DS1307 rtc;
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);  // A4:sda A5:scl

// LED thing
#define LED0 0
#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4
#define LED5 5

bool led0On = false;
bool led1On = false;
bool led2On = false;
bool led3On = false;
bool led4On = false;
bool led5On = false;

// LED switches thing
#define LED0SW A0
#define LED1SW A1
#define LED2SW A2
#define LED3SW A3

int led0Flag = LOW;
int led1Flag = LOW;
int led2Flag = LOW;
int led3Flag = LOW;

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

// Alarm thing
bool alarmSetup = false;
bool toggleTime = false; // if false then change hour, true change minute

int alarmHour = 0;
int alarmMinute = 0;
int alarmSecond = 0;

// Functions
void drawAlarm(void) {

  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(0, 11, "Setting alarm");

  // Print time
  u8g.setFont(u8g_font_courB18);
  u8g.drawStr(0, Y_OFFSET, String(alarmHour).c_str());
  u8g.drawStr(SLOT_WIDTH + 10, Y_OFFSET, ":");
  u8g.drawStr(SLOT_WIDTH * 2, Y_OFFSET, String(alarmMinute).c_str());
  u8g.drawStr(SLOT_WIDTH * 3 + 10, Y_OFFSET, ":");
  u8g.drawStr(SLOT_WIDTH * 4, Y_OFFSET, String(alarmSecond).c_str());
}

void drawClock(void) {
  DateTime now = rtc.now();

  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(0, 11, "Ho Chi Minh");

  // Print time
  u8g.setFont(u8g_font_courB18);
  u8g.drawStr(0, Y_OFFSET, String(now.hour()).c_str());
  u8g.drawStr(SLOT_WIDTH + 10, Y_OFFSET, ":");
  u8g.drawStr(SLOT_WIDTH * 2, Y_OFFSET, String(now.minute()).c_str());
  u8g.drawStr(SLOT_WIDTH * 3 + 10, Y_OFFSET, ":");
  u8g.drawStr(SLOT_WIDTH * 4, Y_OFFSET, String(now.second()).c_str());
}

void clock(char key) {
  if (key == '0') {
    alarmSetup = !alarmSetup;
  }

  if (key == 'C') {
      alarmHour = 0;
      alarmMinute = 0;
  }

  if (alarmSetup) {
    if (key == 'D') {
      toggleTime = !toggleTime;
    }

    if (key == '+') {
      if (!toggleTime) {
          alarmHour += 1;
      } else {
          alarmMinute += 1;
      }
    }

    if (key == '-') {
      if (!toggleTime) {
          alarmHour -= 1;
      } else {
          alarmMinute -=1;
      }
    }
  }

  // Boundchecking
  alarmHour = (alarmHour > 24) ? 24 : alarmHour;
  alarmHour = (alarmHour < 0) ? 0 : alarmHour;

}

void led(char key) {
  if (key == '1') {
    if (led0On) {
      led0On = false;
      digitalWrite(LED0, LOW);
    } else {
      led0On = true;
      digitalWrite(LED0, HIGH);
    }
  }

  if (key == '2') {
    if (led1On) {
      led1On = false;
      digitalWrite(LED1, LOW);
    } else {
      led1On = true;
      digitalWrite(LED1, HIGH);
    }
  }

  if (key == '3') {
    if (led2On) {
      led2On = false;
      digitalWrite(LED2, LOW);
    } else {
      led2On = true;
      digitalWrite(LED2, HIGH);
    }
  }

  if (key == '4') {
    if (led3On) {
      led3On = false;
      digitalWrite(LED3, LOW);
    } else {
      led3On = true;
      digitalWrite(LED3, HIGH);
    }
  }

  if (key == '5') {
    if (led4On) {
      led4On = false;
      digitalWrite(LED4, LOW);
    } else {
      led4On = true;
      digitalWrite(LED4, HIGH);
    }
  }

  if (key == '6') {
    if (led5On) {
      led5On = false;
      digitalWrite(LED5, LOW);
    } else {
      led5On = true;
      digitalWrite(LED5, HIGH);
    }
  }
}

void ledsw(void) {
  int led0reading = digitalRead(LED0SW);
  if (led0reading != led0Flag) {
    if (led0On) {
      digitalWrite(LED0, LOW);
    } else {
      digitalWrite(LED0, HIGH);
    }
    led0On = !led0On;
  }
  led0Flag = led0reading;

  int led1reading = digitalRead(LED1SW);
  if (led1reading != led1Flag) {
    if (led1On) {
      digitalWrite(LED1, LOW);
    } else {
      digitalWrite(LED1, HIGH);
    }
    led1On = !led1On;
  }
  led1Flag = led1reading;

  int led2reading = digitalRead(LED2SW);
  if (led2reading != led2Flag) {
    if (led2On) {
      digitalWrite(LED2, LOW);
    } else {
      digitalWrite(LED2, HIGH);
    }
    led2On = !led2On;
  }
  led2Flag = led2reading;

  int led3reading = digitalRead(LED3SW);
  if (led3reading != led3Flag) {
    if (led3On) {
      digitalWrite(LED3, LOW);
    } else {
      digitalWrite(LED3, HIGH);
    }
    led3On = !led3On;
  }
  led3Flag = led3reading;
}

void setup(void) {
  //Serial.begin(9600);

  // LED setup
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

  // LEDSW setup 
  pinMode(LED0SW, INPUT_PULLUP);
  pinMode(LED1SW, INPUT_PULLUP);
  pinMode(LED2SW, INPUT_PULLUP);
  pinMode(LED3SW, INPUT_PULLUP);

  // RTC Setup
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // OLED screen setup
  // assign default color value
  if (u8g.getMode() == U8G_MODE_R3G3B2) {
    u8g.setColorIndex(255);  // white
  } else if (u8g.getMode() == U8G_MODE_GRAY2BIT) {
    u8g.setColorIndex(3);  // max intensity
  } else if (u8g.getMode() == U8G_MODE_BW) {
    u8g.setColorIndex(1);  // pixel on
  } else if (u8g.getMode() == U8G_MODE_HICOLOR) {
    u8g.setHiColorByRGB(255, 255, 255);
  }
}

void loop(void) {
  // picture loop
  u8g.firstPage();
  do {
    if (alarmSetup) {
      drawAlarm();
    } else {
      drawClock();
    }
  } while (u8g.nextPage());

  char key = keypad.getKey();
  Serial.println(key);
  clock(key);
  led(key);
  ledsw();

  // rebuild the picture after some delay
  delay(50);
}
