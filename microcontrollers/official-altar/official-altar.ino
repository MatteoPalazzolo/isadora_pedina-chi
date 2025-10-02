// WIFI
#include <WiFi.h>
// NFC
#include <SPI.h>
#include <MFRC522.h>
// LED
#include <Adafruit_NeoPixel.h> // FastLED && Adafruit NeoPixel Libraries
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

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

// LED BORDI
#define LED_BORDI_PIN 13
#define LED_BORDI_COUNT 54
// LED SCRITTA
#define LED_SCRITTA_PIN 12
#define LED_SCRITTA_COUNT 14

MFRC522 rfid1(SS_1, RST_PIN);
MFRC522 rfid2(SS_2, RST_PIN);
MFRC522 rfid3(SS_3, RST_PIN);
int checkReader(MFRC522 &reader, int placeId);

WiFiServer server(PORT);
WiFiClient client;

Adafruit_NeoPixel ledBordi(LED_BORDI_COUNT, LED_BORDI_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledScritta(LED_SCRITTA_COUNT, LED_SCRITTA_PIN, NEO_GRB + NEO_KHZ800);

bool active = false;
bool waitForAck = false;

void onActiveTrue() {
  // scritta
  for (int pixel = 0; pixel < LED_SCRITTA_COUNT; pixel++) {
    ledScritta.setPixelColor(pixel, 255, 255, 255);
  }
  ledScritta.show();
  // bordi
  ledAnimationPowerOn();
}

void onActiveFalse() {
  ledScritta.clear();
  ledScritta.show();
  ledBordi.clear();
  ledBordi.show();
}

void ledAnimationPowerOn() {
  ledBordi.clear();
  const int z = 2; 
  for (int k = 0; k < 10; k++) {
    for (int j = 0; j < z; j++) {
      for (int i = 0; i < LED_BORDI_COUNT; i++) {
        if (i % z == j) {
          ledBordi.setPixelColor(i, ledBordi.Color(221, 85, 255)); // pink
        } else {
          // ledBordi.setPixelColor(i, ledBordi.Color(84, 94, 178)); // purple
          ledBordi.setPixelColor(i, ledBordi.Color(65, 225, 90)); // green
        }
      }
      ledBordi.show();
      delay(500);
    }
  }
  ledBordi.clear();
  ledBordi.show();

}

bool flip = false;
void handleAck(int ack) {
  if (ack == 0) {
    // cambio di led, ma neutro
    flip = !flip;
    for (int i = 0; i < LED_BORDI_COUNT; i++) {
      if (i % 2 == flip) {
        ledBordi.setPixelColor(i, ledBordi.Color(221, 85, 255)); // pink
      } else {
        ledBordi.setPixelColor(i, ledBordi.Color(65, 225, 90)); // green
      }
    }
    ledBordi.show();
  }
  else if (ack == 1) {
    // cambio di led a rosso
    for (int pixel = 0; pixel < LED_BORDI_COUNT; pixel++) {
      ledBordi.setPixelColor(pixel, 231, 21, 77);
    }
    ledBordi.show();
  }
  else if (ack == 2) {
    // cambio di led a verde
    for (int pixel = 0; pixel < LED_BORDI_COUNT; pixel++) {
      ledBordi.setPixelColor(pixel, 54, 176, 74);
    }
    ledBordi.show();

    delay(1000);

    active = false;
    onActiveFalse();
    digitalWrite(2, LOW);
    Serial.println("System: OFF");

  }
}

void setupWiFi() {
  pinMode(2, OUTPUT);
  
  Serial.println();
  Serial.print("Connessione a ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    Serial.print(".");
  }
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);

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

void setupLED() {
  // scritta
  ledScritta.begin();
  ledScritta.setBrightness(75);
  ledScritta.clear();
  ledScritta.show();
  // bordi
  ledBordi.begin();
  ledBordi.setBrightness(10);
  ledBordi.clear();
  ledBordi.show();
}

void setup() {
  Serial.begin(9600);
  setupWiFi();
  setupNFC();
  setupLED();
}

void sendTCP(String msg) {
  Serial.print("TCP SEND: ");
  Serial.println(msg);
  if (!client || !client.connected()) {
    Serial.println("ERRORE: Client non connesso!");
    return;
  }
  // client.print(msg + '\n');
  size_t sent = client.print(msg + '\n');
  if (sent == 0) {
    Serial.println("ERRORE: Invio fallito. Chiudo client.");
    client.stop(); // forza la chiusura del client
    Serial.println(client.connected());
  }
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
  // Serial.println(uid);

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

String lastCode = "";
unsigned long pingMillis = 0;

void loop() {
  // se non c'è nessun client connesso prova a connetterti
  if (!client || !client.connected()) {
    client = server.available();
    if (client) {
      Serial.println("Client connected!");
    } else {
      Serial.println("Looking for client!");
      return;
    }
  }

  // assicurati di mandare un PING ogni 20 sec per evitare che la connessione si chiuda
  if (millis() - pingMillis >= 20000) {
    sendTCP("PING");
    pingMillis = millis();
  }

  // controlla se il client sta comunicando
  if (client.available()) {
    String msg = client.readStringUntil('\n');
    msg.trim();

    Serial.print("RCV: ");
    Serial.println(msg);

    if (msg.startsWith("POWER:ON") && !active) {
      active = true;
      onActiveTrue();
      digitalWrite(2, HIGH);
      Serial.println("System: ON");
      sendTCP("ACK:7");
    } else if (msg.startsWith("POWER:OFF") && active) {
      active = false;
      onActiveFalse();
      digitalWrite(2, LOW);
      Serial.println("System: OFF");
      sendTCP("ACK:6");
    } else if (msg.startsWith("ACK:") && active && waitForAck) {
      int sepIndex = msg.indexOf(':');
      String ackStr = msg.substring(sepIndex + 1);
      ackStr.trim();
      handleAck(ackStr.toInt());
      waitForAck = false;
    }
    
  }

  if (!active) {
    return;
  }

  int read1 = checkReader(rfid1, 0);
  int read2 = checkReader(rfid2, 0);
  int read3 = checkReader(rfid3, 0);

  String code = String(read1) + String(read2) + String(read3);
  if (code != lastCode && !waitForAck) {
    lastCode = code;
    Serial.println(code);
    sendTCP("CODE:" + code);
    waitForAck = true;
  }

}
