// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;

// Speed settings
const int lowSpeed = 100;
const int mediumSpeed = 150;
const int maxSpeed = 255;

// Analog input pin
int analogInputPin1 = A0;
int analogInputPin2 = A1;
int analogInputPin3 = A2;
int analogInputPin4 = A3;

int buttonPin = 4;
int ledPin = 3;

bool buttonState = false;

void setup() {
  pinMode(buttonPin,INPUT);
  pinMode(ledPin,OUTPUT);

  // Set motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Set analog input pins as input
  pinMode(analogInputPin1, INPUT);
  pinMode(analogInputPin2, INPUT);
  pinMode(analogInputPin3, INPUT);
  pinMode(analogInputPin4, INPUT);

  // Turn off Motor A - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if ( buttonState == true){
    digitalWrite(ledPin, HIGH);
  }
  else {
    digitalWrite(ledPin, LOW);
  }

  speedControl();
}

// This function lets you control spinning direction of Motor A
void directionControl() {
  // Set Motor A to medium speed
  analogWrite(enA, mediumSpeed);

  // Turn on Motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  delay(2000);

  // Now change Motor A direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(2000);

  // Turn off Motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

// This function lets you control speed of Motor A
void speedControl() {

  // Check if voltage on analog input pin is greater than 0
  if (analogRead(analogInputPin1) > 100) {
    analogWrite(enA, maxSpeed);
    // Turn on Motor A
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }

  // Check if voltage on analog input pin is greater than 0
  if (analogRead(analogInputPin2) > 100) {
    analogWrite(enA, mediumSpeed);
    // Turn on Motor A
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }

  // Check if voltage on analog input pin is greater than 0
  if (analogRead(analogInputPin3) > 100) {
    analogWrite(enA, lowSpeed);
    // Turn on Motor A
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }

  // Check if voltage on analog input pin is greater than 0
  if (analogRead(analogInputPin4) > 100) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
}
