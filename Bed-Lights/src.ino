// This is the source code for the Arduino

#define LEDPIN 12 // LED pin
#define LIGHTSENSORPIN A0 // Light sensor analog pin
#define LIGHTTHRESHOLD 50 // Threshold where the light sensor will activate
#define IRSENSORPIN 13 // IR pin

bool LEDON = false;
//There's a case where FLights turning on will activate the light sensor and immediately turn the FLights off.
//Got this working by: if FLights on & there's motion, light sensor will always return no lights. 
//And FLights will be off when there's no motion.

void setup() {
  pinMode(LIGHTSENSORPIN, INPUT);
  pinMode(IRSENSORPIN, INPUT);
  pinMode(LEDPIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  float LSensor = analogRead(LIGHTSENSORPIN); // measured in lux
  if (LEDON == true) {
    LSensor = 0; // If led is on then Light sensor will always be off
  }
  float IRSensor = digitalRead(IRSENSORPIN);

  if (LSensor < LIGHTTHRESHOLD && IRSensor == 0) { // If no light and sensor is on (IR on is 0 for some reasons)
    digitalWrite(LEDPIN, HIGH);
    LEDON = true;
  } else {
    digitalWrite(LEDPIN, LOW);
    LEDON = false;
  }
}
