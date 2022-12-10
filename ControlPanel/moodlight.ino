#include <Keypad.h>

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
#define LED4SW A4
#define LED5SW A5

int led0Flag = LOW;
int led1Flag = LOW;
int led2Flag = LOW;
int led3Flag = LOW;
int led4Flag = LOW;
int led5Flag = LOW;

// Keypad thing
const byte ROWS = 3; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
};
byte colPins[COLS] = {8, 7, 6}; //connect to the row pinouts of the keypad
byte rowPins[ROWS] = {11, 10, 9}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void led(char key) {
  // Individual led
  if (key == '2') {
    if (led0On) {
      led0On = false;
      digitalWrite(LED0, LOW);
    } else {
      led0On = true;
      digitalWrite(LED0, HIGH);
    }
  }

  if (key == '3') {
    if (led1On) {
      led1On = false;
      digitalWrite(LED1, LOW);
    } else {
      led1On = true;
      digitalWrite(LED1, HIGH);
    }
  }

  if (key == '4') {
    if (led2On) {
      led2On = false;
      digitalWrite(LED2, LOW);
    } else {
      led2On = true;
      digitalWrite(LED2, HIGH);
    }
  }

  if (key == '5') {
    if (led3On) {
      led3On = false;
      digitalWrite(LED3, LOW);
    } else {
      led3On = true;
      digitalWrite(LED3, HIGH);
    }
  }

  if (key == '6') {
    if (led4On) {
      led4On = false;
      digitalWrite(LED4, LOW);
    } else {
      led4On = true;
      digitalWrite(LED4, HIGH);
    }
  }

  // Mood
  if (key == '1') { // alternating 1
    led0On = true;
    digitalWrite(LED0, HIGH);

    led2On = true;
    digitalWrite(LED2, HIGH);

    led4On = true;
    digitalWrite(LED4, HIGH);

    led1On = false;
    digitalWrite(LED1, LOW);

    led3On = false;
    digitalWrite(LED3, LOW);
  }

  if (key == '7') { // alternating 2
    led1On = true;
    digitalWrite(LED1, HIGH);

    led3On = true;
    digitalWrite(LED3, HIGH);

    led0On = false;
    digitalWrite(LED0, LOW);

    led2On = false;
    digitalWrite(LED2, LOW);

    led4On = false;
    digitalWrite(LED4, LOW);
  }

  if (key == '8') { // all off
    led1On = false;
    digitalWrite(LED1, LOW);

    led3On = false;
    digitalWrite(LED3, LOW);

    led0On = false;
    digitalWrite(LED0, LOW);

    led2On = false;
    digitalWrite(LED2, LOW);

    led4On = false;
    digitalWrite(LED4, LOW);
  }

  if (key == '9') { // all on
    led1On = true;
    digitalWrite(LED1, HIGH);

    led3On = true;
    digitalWrite(LED3, HIGH);

    led0On = true;
    digitalWrite(LED0, HIGH);

    led2On = true;
    digitalWrite(LED2, HIGH);

    led4On = true;
    digitalWrite(LED4, HIGH);
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

  int led4reading = digitalRead(LED4SW);
  if (led4reading != led4Flag) {
    if (led4On) {
      digitalWrite(LED4, LOW);
    } else {
      digitalWrite(LED4, HIGH);
    }
    led4On = !led4On;
  }
  led4Flag = led4reading;

  int led5reading = digitalRead(LED5SW);
  if (led5reading != led5Flag) {
    if (led5On) {
      digitalWrite(LED5, LOW);
    } else {
      digitalWrite(LED5, HIGH);
    }
    led5On = !led5On;
  }
  led5Flag = led5reading;
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
  pinMode(LED4SW, INPUT_PULLUP);
  pinMode(LED5SW, INPUT_PULLUP);
}

void loop(void) {
  char key = keypad.getKey();
  //if (key != 0) Serial.println(key);
  led(key);
  ledsw();

  // rebuild the picture after some delay
  delay(50);
}
