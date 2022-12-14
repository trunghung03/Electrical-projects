//------------------------------------------------------------------------------//
// Change this
#define FADESPEED 5        // Fadespeed
// More is slower, less is faster
// This is the time that the Arduino will wait before decreasing/increasing brightness
// (But it's in milisecond, so it's really fast

#define LIGHTTHRESHOLD 50  // Threshold where the light sensor will activate
// Measured in lux 
// More is brighter, less is darker (50 is about a bright room)
// https://en.wikipedia.org/wiki/Lux (there's a table with the common lux value in different condition)
//------------------------------------------------------------------------------//

#define LEDPIN 3           // LED pin
#define LIGHTSENSORPIN A0  // Light sensor analog pin
#define IRSENSORPIN 5      // IR pin

bool LEDON = false;           // Set variable to keep track of LED status
bool fullBrightness = false;  // Keep track of LED brightness

void fadeLED(boolean turnOn, int fadeSpeed) {
  if (turnOn) {
    for (int i = 0; i < 256; i++) {
      analogWrite(LEDPIN, i);
      delay(fadeSpeed);
    }
    fullBrightness = true;
  } else {
    for (int i = 255; i > -1; i--) {
      analogWrite(LEDPIN, i);
      delay(fadeSpeed);
    }
    fullBrightness = false;
  }
}

void setup(void) {
  Serial.begin(9600);

  pinMode(LIGHTSENSORPIN, INPUT);
  pinMode(IRSENSORPIN, INPUT);
  pinMode(LEDPIN, OUTPUT);
}

void loop(void) {
  // LED loop
  float LSensor = analogRead(LIGHTSENSORPIN);  // measured in lux

  if (LEDON == true) {
    LSensor = 0;  // If led is on then Light sensor will always be off
  }
  float IRSensor = digitalRead(IRSENSORPIN);

  if (LSensor < LIGHTTHRESHOLD && IRSensor == 1) {  // If no light and sensor is on (IR on is 0 for some reasons)
    // LED fades on
    if (!fullBrightness) {  // If light is already on then no fade on
      fadeLED(true, FADESPEED);
    }
    LEDON = true;
  } else {
    // LED fades off
    if (fullBrightness) {  // If light is already off then no fade off
      fadeLED(false, FADESPEED);
    }
    LEDON = false;
  }
}
