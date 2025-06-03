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

WiFiServer server(PORT);
WiFiClient client;

struct Sensor {
  MFRC522& reader;
  int placeId;
  int prvsState = 0;
  int score = 0;
  int zeroCounter = 0;

  Sensor(MFRC522& r, int pid)
    : reader(r), placeId(pid) {}
};

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

int maxScore = 0;
void checkSensor(Sensor& sensor) {
  // riavvia il sensore e aspetta un attimo
  sensor.reader.PCD_Init();
  delay(100);
  
  // trova l'id della pedina sul sensore
  int pwnId = 0;
  if (sensor.reader.PICC_IsNewCardPresent() && sensor.reader.PICC_ReadCardSerial()) {
    String uid = "";
    for (byte i = 0; i < sensor.reader.uid.size; i++) {
      if (sensor.reader.uid.uidByte[i] < 0x10) uid += "0";
      uid += String(sensor.reader.uid.uidByte[i], HEX);
    }
    Serial.println(uid);

    if (uid == "0495c53cc12a81") {
      pwnId = ID_VOLPE;
    }
    else if (uid == "049dc53cc12a81") {
      pwnId = ID_PEDONE;
    }
    else if (uid == "04a1c53cc12a81") {
      pwnId = ID_ROSA;
    }

    sensor.reader.PICC_HaltA();
    sensor.reader.PCD_StopCrypto1();
  }

  // incrementa lo score del sensore se la pedina è cambiata
  if (sensor.prvsState != pwnId) {
    if (pwnId == 0) {
      sensor.zeroCounter += 1;
      if (sensor.zeroCounter >= 5) {
        sensor.score = 0;
        sensor.prvsState = 0;
        sensor.zeroCounter = 0;
      }
    } else {
      maxScore += 1;
      sensor.score = maxScore;
      sensor.prvsState = pwnId;
      sensor.zeroCounter = 0;
    }
  }
  
}

Sensor sensors[] = {
  Sensor(rfid1, ID_SCALE),
  Sensor(rfid2, ID_STRISCE),
  Sensor(rfid3, ID_PARCO)
};

void loop() {

  // check client connection
  if (!client || !client.connected()) {
    client = server.available();
    if (client) {
      Serial.println("Client connected!");
    } else {
      Serial.println("Looking for client!");
      return;
    }
  }

  // controlla tutti i sensori
  for (int i = 0; i < 3; i++) {
    checkSensor(sensors[i]);
  }

  // cerca il sensore con lo score più alto e restituisci il codice corrispondente
  int bestIdx = 0;
  for (int i = 1; i < 3; i++) {
    if (sensors[i].score > sensors[bestIdx].score) {
      bestIdx = i;
    }
  }

  // invia a Isadora
  if (sensors[bestIdx].prvsState == 0) {
    sendTCP("0");
  } else {
    sendTCP(String(sensors[bestIdx].placeId + sensors[bestIdx].prvsState));
  }  

}





