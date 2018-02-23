#include <Adafruit_NeoPixel.h>

#define NEO_PIN 3
#define MIC_0_PIN 2

#define DEALY 30
#define BUTTON_DEBOUNCE_PERIOD 20 //ms
#define LED_COUNT 7

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, NEO_PIN , NEO_GRB + NEO_KHZ800);
void setup() {
  strip.begin();
  strip.show();
  pinMode(MIC_0_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(MIC_0_PIN), checkSound, FALLING);
}

int cnt = 0;
bool soundFlag = true;
long lastTime = 0;

void checkSound() {
  soundFlag = true;
}

void loop() {
  specialDelay(1000);
  if (lastTime + 15000 < millis()) {
    soundFlag = true;
    lastTime = millis();
  }
}

void specialDelay(long timeMs) {
  while (timeMs--) {
    delay(1);
    if (soundFlag == true) {
      rotateVisual();
    }
  }
}

void rotateVisual() {
  switch (cnt++ % 6) {
    case 0:
      colorWipe(strip.Color(random(0, 127), 255, random(0, 127)), DEALY);
      break;
    case 1:
      colorWipe(strip.Color(random(0, 255), random(0, 255), random(0, 255)), DEALY);
      break;
    case 2:
      colorWipe(strip.Color(255, random(0, 255), random(0, 255)), DEALY);
      break;
    case 3:
      colorWipe(strip.Color(random(0, 127), random(0, 127), 255), DEALY);
      break;
    case 4:
      colorWipe(strip.Color(random(0, 127), random(0, 127), 255), DEALY);
      break;
    case 5:
      rainbowCycle(DEALY / 20);
      break;
    case 6:
      theaterChase(strip.Color(random(0, 127), random(0, 127), 255), DEALY * 2); 
      break;
    case 7:
      theaterChase(strip.Color(127, 127, 127), DEALY * 2); 
      colorWipe(strip.Color(0, 0, 0), DEALY); //off
      break;
    case 8:
      theaterChase(strip.Color(127,   random(0, 127),   random(0, 127)), DEALY);
      colorWipe(strip.Color(0, 0, 0), DEALY); //off
      break;
    case 9:
      colorWipe(strip.Color(255, random(0, 255), random(0, 255)), DEALY);
      colorWipe(strip.Color(random(0, 25), 255, random(0, 127)), DEALY);
      colorWipe(strip.Color(random(0, 255), random(0, 255), 255), DEALY);
      colorWipe(strip.Color(random(0, 255), random(0, 255), random(0, 255)), DEALY);
      break;
  }
  colorWipe(strip.Color(0, 0, 0), DEALY); //off
  soundFlag = false;
}


void heartBeat(uint16_t r, uint16_t g, uint16_t b, uint16_t msFrequency) {
  uint16_t steps = msFrequency / DEALY;

  for (uint8_t i = 0; i < steps ; i ++) {
    colorAll(strip.Color(r * i / steps , g * i / steps , b * i / steps));
  }
  for (uint8_t i = steps; i > 0; i --) {
    colorAll(strip.Color(r * i / steps , g * i / steps, b * i / steps));
  }
  colorAll(strip.Color(0, 0, 0));
}

void colorAll(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    if (soundFlag == true) {
      return;
    }
  }
}

void colorAllRandom() {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(random(0, 255), random(0, 255), random(0, 255)));
    delay(DEALY);
    strip.show();
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = strip.numPixels(); i > 0; i--) {
    strip.setPixelColor(i - 1, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;
  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for (j = 0; j < 256 * 5; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   //256가지의 색을 표현
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255));
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);
      }
    }
  }
}

uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
