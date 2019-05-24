/*
 * MoonBot_Eyes.cpp
 *
 *  Created on: 2019年5月23日
 *      Author: ysq
 */
#include "MoonBot_Eyes.h"

void colorFade(Adafruit_NeoPixel& led,
               uint8_t r, uint8_t g, uint8_t b,
               uint8_t wait) {
  uint8_t is_change = 0;
  uint8_t pix_r,pix_g,pix_b;
  uint32_t pix_color;
  do {
    is_change = 0;
    for (uint16_t i = 0; i < led.numPixels(); ++i) {
      pix_color = led.getPixelColor(i);
      pix_r = (pix_color>>16)&0xFF;
      pix_g = (pix_color>>8)&0xFF;
      pix_b = (pix_color)&0xFF;
      if (pix_r != r) {
        pix_r += pix_r>r ? -1:1;
        is_change++;
      }
      if (pix_g != g) {
        pix_g += pix_g>g ? -1:1;
        is_change++;
      }
      if (pix_b != b) {
        pix_b += pix_b>b ? -1:1;
        is_change++;
      }
      led.setPixelColor(i, pix_r, pix_g, pix_b);
    }
    led.show();
    delay(wait);
  } while (is_change);
}

// Fill the dots one after the other with a color
void colorWipe(Adafruit_NeoPixel& led, uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<led.numPixels(); i++) {
    led.setPixelColor(i, c);
    led.show();
    delay(wait);
  }
}

void rainbow(Adafruit_NeoPixel& led, uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<led.numPixels(); i++) {
      led.setPixelColor(i, WheelColor(led, (i+j) & 255));
    }
    led.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(Adafruit_NeoPixel& led, uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< led.numPixels(); i++) {
      led.setPixelColor(i, WheelColor(led, ((i * 256 / led.numPixels()) + j) & 255));
    }
    led.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(Adafruit_NeoPixel& led, uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < led.numPixels(); i=i+3) {
        led.setPixelColor(i+q, c);    //turn every third pixel on
      }
      led.show();

      delay(wait);

      for (uint16_t i=0; i < led.numPixels(); i=i+3) {
        led.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(Adafruit_NeoPixel& led, uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < led.numPixels(); i=i+3) {
        led.setPixelColor(i+q, WheelColor(led, (i+j) % 255));    //turn every third pixel on
      }
      led.show();

      delay(wait);

      for (uint16_t i=0; i < led.numPixels(); i=i+3) {
        led.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t WheelColor(Adafruit_NeoPixel& led, byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return led.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return led.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return led.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}



