#include <MoonBot.h>

// if you want to change eyes port, set the port here(port1-9)
#define MOONBOT_EYES_PORT     kPort1

void setup() {
  moonbot_eyes.setPin(moonbotPortToPin(MOONBOT_EYES_PORT, kPortPin1));
  moonbot_eyes.begin();  // enable MoonBot LED eyes, default port is port 1
  //  clear MoonBot LED eyes
  moonbot_eyes.clear();
  moonbot_eyes.show();
}

void loop() {
  // eyes show rainbow color
  rainbowCycle(moonbot_eyes, 5);
  // eyes cycle
  for (int i = 0; i < 3; ++i) {
    colorWipe(moonbot_eyes, 0, 40);
    colorWipe(moonbot_eyes, 0x3030, 40);
  }
  // eyes flash
  for (int i = 0; i < 3; ++i) {
    theaterChase(moonbot_eyes, 0x3030, 40);
  }
  // eyes blink
  moonbot_eyes.fill(0x3030, 0, 0);
  moonbot_eyes.show();
  delay(1000);
  for (int i = 0; i < 2; ++i) {
    unsigned int action_delay = 30;
    moonbot_eyes.setPixelColor(0, 0);
    moonbot_eyes.setPixelColor(6, 0);
    moonbot_eyes.show();
    delay(action_delay);
    moonbot_eyes.setPixelColor(1, 0);
    moonbot_eyes.setPixelColor(5, 0);
    moonbot_eyes.setPixelColor(7, 0);
    moonbot_eyes.setPixelColor(11, 0);
    moonbot_eyes.show();
    delay(action_delay);
    moonbot_eyes.setPixelColor(2, 0);
    moonbot_eyes.setPixelColor(4, 0);
    moonbot_eyes.setPixelColor(8, 0);
    moonbot_eyes.setPixelColor(10, 0);
    moonbot_eyes.show();
    delay(action_delay);
    moonbot_eyes.setPixelColor(3, 0);
    moonbot_eyes.setPixelColor(9, 0);
    moonbot_eyes.show();
    delay(action_delay);
    moonbot_eyes.setPixelColor(3, 0x3030);
    moonbot_eyes.setPixelColor(9, 0x3030);
    moonbot_eyes.show();
    delay(action_delay);
    moonbot_eyes.setPixelColor(2, 0x3030);
    moonbot_eyes.setPixelColor(4, 0x3030);
    moonbot_eyes.setPixelColor(8, 0x3030);
    moonbot_eyes.setPixelColor(10, 0x3030);
    moonbot_eyes.show();
    delay(action_delay);
    moonbot_eyes.setPixelColor(1, 0x3030);
    moonbot_eyes.setPixelColor(5, 0x3030);
    moonbot_eyes.setPixelColor(7, 0x3030);
    moonbot_eyes.setPixelColor(11, 0x3030);
    moonbot_eyes.show();
    delay(action_delay);
    moonbot_eyes.setPixelColor(0, 0x3030);
    moonbot_eyes.setPixelColor(6, 0x3030);
    moonbot_eyes.show();
  }
  delay(1000);
}

