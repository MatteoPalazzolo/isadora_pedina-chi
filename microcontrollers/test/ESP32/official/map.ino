/*
#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>

// IDS
#define ID_ROSA 1
#define ID_VOLPE 2
#define ID_PEDONE 3
#define ID_SCALE 10
#define ID_PARCO 20
#define ID_STRISCE 30

// DEFAULT PIN
#define SCK_PIN  18 // Arancione
#define MISO_PIN 19 // Verde
#define MOSI_PIN 23 // Giallo
// CUSTOM PIN
#define RST_PIN 22  // Blu
#define SS_1    25  // SDA primo lettore
#define SS_2    26  // SDA secondo lettore
#define SS_3    27  // SDA terzo lettore

// WIFI
// #define SSID "ʕ•ᴥ•ʔ"
// #define PASSWORD "0987654322"
#define SSID "Vodafone-C00830074"
#define PASSWORD "tM4qXXCk3Rr9ytax"
#define PORT 8080

MFRC522 rfid1(SS_1, RST_PIN);
MFRC522 rfid2(SS_2, RST_PIN);
MFRC522 rfid3(SS_3, RST_PIN);
int checkReader(MFRC522 &reader, int placeId);

WiFiServer server(PORT);
WiFiClient client;

void setupWiFi() {
  pinMode(2, OUTPUT);
  
  Serial.println();
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

void setupNFC() {
  SPI.begin();
  rfid1.PCD_Init();
  rfid2.PCD_Init();
  rfid3.PCD_Init();
  Serial.println("Lettori NFC inizializzati.");
}

void setup() {
  Serial.begin(9600);
  setupWiFi();
  setupNFC();
}

void sendTCP(String msg) {
  Serial.print("TCP SEND: ");
  Serial.println(msg);
  if (!client || !client.connected()) {
    Serial.println("ERRORE: Client non connesso!");
    return;
  }
  client.print(msg + '\n');
}

int checkReader(MFRC522 &reader, int placeId) {

  if (!client || !client.connected()) {
    client = server.available();
    if (client) {
      Serial.println("Client connesso!");
    }
  }

  // read
  // if (client && client.connected() && client.available()) {
  //   String richiesta = client.readStringUntil('\n');
  //   Serial.println("Ricevuto: " + richiesta);
  //   client.println("Messaggio ricevuto: " + richiesta);
  // }
  
  reader.PCD_Init();
  delay(100);
  
  if (!reader.PICC_IsNewCardPresent() || !reader.PICC_ReadCardSerial())
    return 0;

  String uid = "";
  for (byte i = 0; i < reader.uid.size; i++) {
    if (reader.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(reader.uid.uidByte[i], HEX);
  }
  Serial.println(uid);

  if (uid == "04a1c53cc12a81") {
    placeId += ID_ROSA;
  }
  else if (uid == "0495c53cc12a81") {
    placeId += ID_VOLPE;
  }
  else if (uid == "049dc53cc12a81") {
    placeId += ID_PEDONE;
  }
  else {
    placeId = 0;
  }

  reader.PICC_HaltA();
  reader.PCD_StopCrypto1();

  return placeId;
  
}

int lastId = 0;

void loop() {
  int id = 0;
  
  int read1 = checkReader(rfid1, ID_SCALE);
  if (read1 != 0)
    id = read1;

  int read2 = checkReader(rfid2, ID_PARCO);
  if (read2 != 0)
    id = read2;

  int read3 = checkReader(rfid3, ID_STRISCE);
  if (read3 != 0) 
    id = read3;

  if (id != lastId) {
    lastId = id;
  } else {
    sendTCP(String(id));
  }

}
*/