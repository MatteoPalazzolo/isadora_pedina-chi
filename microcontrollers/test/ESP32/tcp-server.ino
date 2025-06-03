/*
#include <WiFi.h>

// #define SSID "Vodafone-C00830074"
// #define PASSWORD "tM4qXXCk3Rr9ytax"

// WIFI
#define SSID "ʕ•ᴥ•ʔ"
#define PASSWORD "0987654322"
#define PORT 8080

WiFiServer server(PORT);
WiFiClient client;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT); 

  Serial.print("Connessione a ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

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
  
  server.begin();
  Serial.println("Server TCP avviato");
}

void loop() {
  if (!client || !client.connected()) {
    client = server.available();
    if (client) {
      Serial.println("Client connesso!");
    }
  }

  if (client && client.connected() && client.available()) {
    String richiesta = client.readStringUntil('\n');
    Serial.println("Ricevuto: " + richiesta);
    client.println("Messaggio ricevuto: " + richiesta);
  }

}
*/