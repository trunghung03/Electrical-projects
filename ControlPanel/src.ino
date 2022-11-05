#include <U8glib.h> // OLED Display library
#include <virtuabotixRTC.h> // Clock library

#include "DHT.h"

// Temperature/humidity sensor
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Format OLED display
#define SLOT_WIDTH 20
#define Y_OFFSET 32

// Timezone selection pins
#define BUTTON1 9
#define BUTTON2 10
#define BUTTON3 11
#define BUTTON4 12

#define LEDPIN 3          // LED pin
#define FADESPEED 5      // LED fade speed
#define LIGHTSENSORPIN A0  // Light sensor analog pin
#define LIGHTTHRESHOLD 50  // Threshold where the light sensor will activate
#define IRSENSORPIN 5     // IR pin

U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);	// I2C / TWI 
virtuabotixRTC myRTC(6, 7, 8); // Create real time clock object

enum place {HCM, NY, LON, SYD}; // List of selectable timezone
enum place currentPlace = HCM; // Default timezone
bool LEDON = false; // Set variable to keep track of LED status
bool fullBrightness = false; // Keep track of LED brightness

inline int positive_modulo(int i, int n) {
  return (i % n + n) % n;
}

void fadeLED(boolean turnOn, int fadeSpeed) {
  if (turnOn) {
    for (int i = 0; i < 256; i++) {
      analogWrite(LEDPIN, i);
      delay(fadeSpeed);
    }
    fullBrightness = true;
  }
  else {
    for (int i = 255; i > -1; i--) {
      analogWrite(LEDPIN, i);
      delay(fadeSpeed);
    }
    fullBrightness = false;
  }
}

enum place getPlace() {
  if (digitalRead(BUTTON1) == LOW) {
    return HCM;
  }
  else if (digitalRead(BUTTON2) == LOW) {
    return NY;
  }
  else if (digitalRead(BUTTON3) == LOW) {
    return LON;
  }
  else if (digitalRead(BUTTON4) == LOW) {
    return SYD;
  }
}

void draw(void) {
  u8g.setFont(u8g_font_unifont);
  uint8_t Hrs = myRTC.hours;

  // Print place depending on current selection
  currentPlace = getPlace();

  switch (currentPlace) {
    case HCM: 
    u8g.drawStr(0, 11, "Ho Chi Minh");
    break;

    case NY: 
    u8g.drawStr(0, 11, "New York");
    Hrs = positive_modulo(Hrs - 11, 24); //Used a special function to make sure there's no negative modulo
    break;

    case LON: 
    u8g.drawStr(0, 11, "London");
    Hrs = positive_modulo(Hrs - 6, 24);
    break;

    case SYD: 
    u8g.drawStr(0, 11, "Sydney");
    Hrs = positive_modulo(Hrs + 4, 24);
    break;
  }

  // Print temperature
  int temp = static_cast<int>(dht.readTemperature());
  String tempformatted = String(temp) + " C";
  u8g.drawStr(95, 11, tempformatted.c_str());

  // Hack to print degree symbol
  u8g.setFont(u8g_font_7x14B);
  u8g.setPrintPos(112, 11);
  u8g.write(0xB0);

  // Print time
  u8g.setFont(u8g_font_courB18);
  u8g.drawStr(0, Y_OFFSET, String(Hrs).c_str());
  u8g.drawStr(SLOT_WIDTH + 10, Y_OFFSET, ":");
  u8g.drawStr(SLOT_WIDTH * 2, Y_OFFSET, String(myRTC.minutes).c_str());
  u8g.drawStr(SLOT_WIDTH * 3 + 10, Y_OFFSET, ":");
  u8g.drawStr(SLOT_WIDTH * 4, Y_OFFSET, String(myRTC.seconds).c_str());
}

void setup(void) {
  // Set temperature sensor
  dht.begin();

  //Set Pull up resistor
  pinMode(BUTTON1, INPUT);
  digitalWrite(BUTTON1, HIGH);

  pinMode(BUTTON2, INPUT);
  digitalWrite(BUTTON2, HIGH);

  pinMode(BUTTON3, INPUT);
  digitalWrite(BUTTON3, HIGH);

  pinMode(BUTTON4, INPUT);
  digitalWrite(BUTTON4, HIGH);

  //myRTC.setDS1302Time(0, 21, 13, 6, 14, 10, 2022);
  // seconds, minutes, hours, day of the week, day of the month, month, year
  // Setup once to sync the RTC clock

  pinMode(LIGHTSENSORPIN, INPUT);
  pinMode(IRSENSORPIN, INPUT);
  pinMode(LEDPIN, OUTPUT);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}

void loop(void) {
  // picture loop
  u8g.firstPage();
  do {
    myRTC.updateTime();
    draw();
  } while( u8g.nextPage() );

  // LED loop
  float LSensor = analogRead(LIGHTSENSORPIN);  // measured in lux

  if (LEDON == true) {
    LSensor = 0;  // If led is on then Light sensor will always be off
  }
  float IRSensor = digitalRead(IRSENSORPIN);

  if (LSensor < LIGHTTHRESHOLD && IRSensor == 1) {  // If no light and sensor is on (IR on is 0 for some reasons)
    // LED fades on
    if (!fullBrightness) { // If light is already on then no fade on
      fadeLED(true, FADESPEED);
    }
    LEDON = true;
  } else {
    // LED fades off
    if (fullBrightness) { // If light is already off then no fade off
      fadeLED(false, FADESPEED);
    }
    LEDON = false;
  }
  
  // rebuild the picture after some delay
  delay(50); 
}

