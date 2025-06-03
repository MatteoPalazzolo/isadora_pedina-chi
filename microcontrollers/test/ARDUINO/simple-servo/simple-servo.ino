#include <Servo.h>

#define SERVO_PIN 5

Servo servo;

void setup() {
  servo.attach(SERVO_PIN);
   
}

void loop() {
  while (true) {
    break;
    servo.write(0);
    delay(5000);
    servo.write(180);
    delay(5000);
  }

}