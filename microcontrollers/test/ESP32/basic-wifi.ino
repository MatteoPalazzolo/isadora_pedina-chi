/*
#include <WiFi.h>

const char* ssid     = "Vodafone-C00830074";
const char* password = "tM4qXXCk3Rr9ytax";

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT); 

  delay(1000);

  Serial.println();
  Serial.print("Connessione a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    Serial.print(".");
  }

  digitalWrite(2, HIGH);
  Serial.println();
  Serial.println("Connesso al WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Nulla per ora
}
*/