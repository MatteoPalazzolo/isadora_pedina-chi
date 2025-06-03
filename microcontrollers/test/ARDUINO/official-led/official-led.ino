#include <Adafruit_NeoPixel.h> // FastLED && Adafruit NeoPixel Libraries
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN_WS2812B 4  // Arduino pin that connects to WS2812B
#define NUM_PIXELS 14  // The number of LEDs (pixels) on WS2812B

Adafruit_NeoPixel WS2812B(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

void setup() {
  WS2812B.begin();
  WS2812B.setBrightness(3);
}

void ledAnimationPowerOn() {
  WS2812B.clear();

  for (int k = 0; k < 10; k++) {
    for (int j = 0; j < 2; j++) {
      for (int i = 0; i < NUM_PIXELS; i++) {
        if (i % 2 == j) {
          WS2812B.setPixelColor(i, WS2812B.Color(221, 85, 255)); // pink
        } else {
          // WS2812B.setPixelColor(i, WS2812B.Color(84, 94, 178)); // purple
          WS2812B.setPixelColor(i, WS2812B.Color(65, 225, 90)); // green
        }
      }
      WS2812B.show();
      delay(500);
    }
  }
  
  WS2812B.clear();
  WS2812B.show();

}

void ledAnimationRightCode() {
  WS2812B.clear();

}

void ledAnimationWrongCode() {
  WS2812B.clear();
  for (int i = 0; i < 5; i++) {
    for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
      WS2812B.setPixelColor(pixel, WS2812B.Color(238, 24, 24));
    }
    WS2812B.show();

    delay(400);

    WS2812B.clear();
    WS2812B.show();
    
    delay(400);
  }


}

void loop() {
  ledAnimationPowerOn();
  delay(5000);
  ledAnimationWrongCode();
  delay(5000);
}
