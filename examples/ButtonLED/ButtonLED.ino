#include <MoonBot.h>

void setup() {
  LED.begin();  // enable controller LED
  //  clear LED color
  LED.clear();
  LED.show();
}

void loop() {
  if (digitalRead(MOONBOT_PIN_BUTTON_A) == LOW
      && digitalRead(MOONBOT_PIN_BUTTON_B) == LOW) {
    // if both button A&B pressed
    // LED0&1 show color cyan
    LED.setPixelColor(0, 0x003030);
    LED.setPixelColor(1, 0x003030);
    LED.show();
  } else if (digitalRead(MOONBOT_PIN_BUTTON_A) == LOW) {
    // if button A pressed
    // LED0 show color green
    LED.setPixelColor(0, 0x003000);
    LED.setPixelColor(1, 0);
    LED.show();
  } else if (digitalRead(MOONBOT_PIN_BUTTON_B) == LOW) {
    // if button B pressed
    // LED1 show color blue
    LED.setPixelColor(0, 0);
    LED.setPixelColor(1, 0x000030);
    LED.show();
  } else {
    // LED1 close
    LED.setPixelColor(0, 0);
    LED.setPixelColor(1, 0);
    LED.show();
  }
}

