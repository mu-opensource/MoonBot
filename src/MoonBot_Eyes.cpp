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

void MoonBotEyesLook(Adafruit_NeoPixel& led,
                     moonbot_look_t look_tpye,
                     uint32_t color) {
  led.clear();
  switch (look_tpye) {
    case kEyesLookUp:
      // left led
      led.setPixelColor(0, color);
      led.setPixelColor(1, color);
      led.setPixelColor(5, color);
      // right led
      led.setPixelColor(6, color);
      led.setPixelColor(7, color);
      led.setPixelColor(11, color);
      break;
    case kEyesLookDown:
      // left led
      led.setPixelColor(2, color);
      led.setPixelColor(3, color);
      led.setPixelColor(4, color);
      // right led
      led.setPixelColor(8, color);
      led.setPixelColor(9, color);
      led.setPixelColor(10, color);
      break;
    case kEyesLookLeft:
      // left led
      led.setPixelColor(0, color);
      led.setPixelColor(3, color);
      led.setPixelColor(4, color);
      led.setPixelColor(5, color);
      // right led
      led.setPixelColor(6, color);
      led.setPixelColor(9, color);
      led.setPixelColor(10, color);
      led.setPixelColor(11, color);
      break;
    case kEyesLookRight:
      // left led
      led.setPixelColor(0, color);
      led.setPixelColor(3, color);
      led.setPixelColor(1, color);
      led.setPixelColor(2, color);
      // right led
      led.setPixelColor(6, color);
      led.setPixelColor(9, color);
      led.setPixelColor(7, color);
      led.setPixelColor(8, color);
      break;
    default:
      return;
  }
  led.show();
}

void MoonBotEyesScroll(Adafruit_NeoPixel& led,
                       moonbot_eyes_scroll_t scroll_tpye,
                       uint32_t color,
                       uint8_t wait) {
  uint32_t a_color[5] = {
      Adafruit_NeoPixel::Color(((color>>16)&0xFF)/4, ((color>>8)&0xFF)/4, (color&0xFF)/4),
      Adafruit_NeoPixel::Color(((color>>16)&0xFF)/2, ((color>>8)&0xFF)/2, (color&0xFF)/2),
      color,
      Adafruit_NeoPixel::Color(((color>>16)&0xFF)/2, ((color>>8)&0xFF)/2, (color&0xFF)/2),
      Adafruit_NeoPixel::Color(((color>>16)&0xFF)/4, ((color>>8)&0xFF)/4, (color&0xFF)/4),
  };
  switch (scroll_tpye) {
    case kEyesScrollUp: {
      for (int i = 0; i < 9; ++i) {
        led.clear();
        if (i < 5) {
          led.setPixelColor(3, a_color[i]);
          led.setPixelColor(9, a_color[i]);
        }
        if (i > 0 && i < 6) {
          led.setPixelColor(2, a_color[i-1]);
          led.setPixelColor(4, a_color[i-1]);
          led.setPixelColor(8, a_color[i-1]);
          led.setPixelColor(10, a_color[i-1]);
        }
        if (i > 1 && i < 7) {
          led.setPixelColor(1, a_color[i-2]);
          led.setPixelColor(5, a_color[i-2]);
          led.setPixelColor(7, a_color[i-2]);
          led.setPixelColor(11, a_color[i-2]);
        }
        if (i > 2 && i < 8) {
          led.setPixelColor(0, a_color[i-3]);
          led.setPixelColor(6, a_color[i-3]);
        }
        led.show();
        delay(wait);
      }
    }
      break;
    case kEyesScrollDown: {
      for (int i = 0; i < 9; ++i) {
        led.clear();
        if (i > 2 && i < 8) {
          led.setPixelColor(3, a_color[i-3]);
          led.setPixelColor(9, a_color[i-3]);
        }
        if (i > 1 && i < 7) {
          led.setPixelColor(2, a_color[i-2]);
          led.setPixelColor(4, a_color[i-2]);
          led.setPixelColor(8, a_color[i-2]);
          led.setPixelColor(10, a_color[i-2]);
        }
        if (i > 0 && i < 6) {
          led.setPixelColor(1, a_color[i-1]);
          led.setPixelColor(5, a_color[i-1]);
          led.setPixelColor(7, a_color[i-1]);
          led.setPixelColor(11, a_color[i-1]);
        }
        if (i < 5) {
          led.setPixelColor(0, a_color[i]);
          led.setPixelColor(6, a_color[i]);
        }
        led.show();
        delay(wait);
      }
    }
      break;
    case kEyesScrollLeft: {
      for (int i = 0; i < 11; ++i) {
        led.clear();
        if (i > 4 && i < 10) {
          led.setPixelColor(10, a_color[i-5]);
          led.setPixelColor(11, a_color[i-5]);
        }
        if (i > 3 && i < 9) {
          led.setPixelColor(6, a_color[i-4]);
          led.setPixelColor(9, a_color[i-4]);
        }
        if (i > 2 && i < 8) {
          led.setPixelColor(7, a_color[i-3]);
          led.setPixelColor(8, a_color[i-3]);
        }
        if (i > 1 && i < 7) {
          led.setPixelColor(4, a_color[i-2]);
          led.setPixelColor(5, a_color[i-2]);
        }
        if (i > 0 && i < 6) {
          led.setPixelColor(0, a_color[i-1]);
          led.setPixelColor(3, a_color[i-1]);
        }
        if (i < 5) {
          led.setPixelColor(1, a_color[i]);
          led.setPixelColor(2, a_color[i]);
        }
        led.show();
        delay(wait);
      }
    }
      break;
    case kEyesScrollRight: {
      for (int i = 0; i < 11; ++i) {
        led.clear();
        if (i < 5) {
          led.setPixelColor(10, a_color[i]);
          led.setPixelColor(11, a_color[i]);
        }
        if (i > 0 && i < 6) {
          led.setPixelColor(6, a_color[i-1]);
          led.setPixelColor(9, a_color[i-1]);
        }
        if (i > 1 && i < 7) {
          led.setPixelColor(7, a_color[i-2]);
          led.setPixelColor(8, a_color[i-2]);
        }
        if (i > 2 && i < 8) {
          led.setPixelColor(4, a_color[i-3]);
          led.setPixelColor(5, a_color[i-3]);
        }
        if (i > 3 && i < 9) {
          led.setPixelColor(0, a_color[i-4]);
          led.setPixelColor(3, a_color[i-4]);
        }
        if (i > 4 && i < 10) {
          led.setPixelColor(1, a_color[i-5]);
          led.setPixelColor(2, a_color[i-5]);
        }
        led.show();
        delay(wait);
      }
    }
      break;
    default:
      return;
  }
}

void MoonBotEyesCircle(Adafruit_NeoPixel& led,
                       uint32_t color,
                       moonbot_eyes_t eyes_type,
                       uint8_t wait) {
  uint8_t r = (color>>16)&0xFF;
  uint8_t g = (color>>8)&0xFF;
  uint8_t b = color&0xFF;
  for (int i = 0; i < 6; ++i) {
    if (eyes_type != kEyesLeft) {
      for (int j = 0; j < 6; ++j) {
        led.setPixelColor((i+j+2)%6, Adafruit_NeoPixel::Color(r*(j+1)/6, g*(j+1)/6, b*(j+1)/6));
      }
    }
    if (eyes_type != kEyesRight) {
      for (int j = 5; j >= 0; --j) {
        led.setPixelColor(6+(5-i+j)%6, Adafruit_NeoPixel::Color(r*(6-j)/6, g*(6-j)/6, b*(6-j)/6));
      }
    }
    led.show();
    delay(wait);
  }
}
