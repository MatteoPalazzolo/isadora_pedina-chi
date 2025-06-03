#include <Stepper.h>

// Define the number of steps per revolution for the motor
const int stepsPerRevolution = 2048;

// Create a Stepper object
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  // Set the speed of the motor to 15 RPM
  myStepper.setSpeed(15);
  Serial.begin(9600);
  Serial.println("Stepper Motor Ready");
}

void loop() {
  // Rotate the motor clockwise
  Serial.println("Rotating clockwise");
  myStepper.step(stepsPerRevolution);
  delay(1000);

  // Rotate the motor counterclockwise
  Serial.println("Rotating counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(1000);
}