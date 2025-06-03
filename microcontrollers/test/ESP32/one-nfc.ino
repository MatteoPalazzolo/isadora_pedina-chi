/*
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         22
#define SS_PIN          27

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  // SPI.begin(18, 19, 23); // SCK, MISO, MOSI
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      //MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      //Serial.print("RFID/NFC Tag Type: ");
      //Serial.println(rfid.PICC_GetTypeName(piccType));

      //
      Serial.print("0x");
      for (int i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
        Serial.print(rfid.uid.uidByte[i], HEX);
      }
      Serial.println();
      //

      uint32_t uidValue = 0;
      for (int i = 0; i < rfid.uid.size; i++) {
        uidValue = uidValue << 8 | rfid.uid.uidByte[i];
      }

      Serial.println(uidValue);

      // switch (uidValue) {
      //   case 0xC3F02E16:
      //     Serial.println("11");
      //     break;
      //   case 0x437E4603:
      //     Serial.println("12");
      //     break;
      //   case 0xF6E1BC02:
      //     Serial.println("13");
      //     break;
      // }


      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }
  delay(100);
}
*/