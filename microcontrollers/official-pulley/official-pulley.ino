#include <Stepper.h>

// Define the number of steps per revolution for the motor
const int stepsPerRevolution = 2048;

// Create a Stepper object
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  // Set the speed of the motor to 15 RPM
  myStepper.setSpeed(15);
  Serial.begin(9600);
  Serial.println();
  Serial.println("Stepper Motor Ready");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    Serial.print("Hai inserito: ");
    Serial.println(input);

    // Se vuoi convertirlo in numero intero
    int value = input.toInt();
    Serial.print("Convertito in intero: ");
    Serial.println(value);

    if (value == 1) {
      Serial.println("Rotating clockwise");
      myStepper.step(stepsPerRevolution);
    } else if (value == 2) {
      Serial.println("Rotating counterclockwise");
      myStepper.step(-stepsPerRevolution);
    }
  }
}