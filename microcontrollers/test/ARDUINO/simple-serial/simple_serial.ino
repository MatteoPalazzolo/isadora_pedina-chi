void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
}

void loop() {
  int id = random(1, 3) * 10 + random(1, 3); 
  Serial.println(id);
  delay(3000);
}
