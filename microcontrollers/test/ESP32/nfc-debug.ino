
#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>

// DEFAULT PIN
#define SCK_PIN  18 // Arancione
#define MISO_PIN 19 // Verde
#define MOSI_PIN 23 // Giallo
// CUSTOM PIN
#define RST_PIN 22  // Blu
#define SS_1    25  // SDA primo lettore
#define SS_2    26  // SDA secondo lettore
#define SS_3    27  // SDA terzo lettore

MFRC522 rfid1(SS_1, RST_PIN);
MFRC522 rfid2(SS_2, RST_PIN);
MFRC522 rfid3(SS_3, RST_PIN);
int checkReader(MFRC522 &reader, int placeId);

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  SPI.begin();
  rfid1.PCD_Init();
  rfid2.PCD_Init();
  rfid3.PCD_Init();
  Serial.println("Lettori NFC inizializzati.");
}

int checkReader(MFRC522 &reader, int placeId) {

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
  
  digitalWrite(2, HIGH);
  delay(100);
  digitalWrite(2, LOW);

  reader.PICC_HaltA();
  reader.PCD_StopCrypto1();

  return placeId;
  
}

void loop() {
  int id = 0;
  
  int read1 = checkReader(rfid1, 0);
  if (read1 != 0)
    id = read1;

  int read2 = checkReader(rfid2, 0);
  if (read2 != 0)
    id = read2;

  int read3 = checkReader(rfid3, 0);
  if (read3 != 0) 
    id = read3;

}
