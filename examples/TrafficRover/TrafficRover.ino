#include <MoonBot.h>

#define LEFT_AVOID_PORT       kPort7
#define RIGHT_AVOID_PORT      kPort3

uint8_t tone_pin = MOONBOT_PIN_BUZZER_SIG;
uint8_t left_avoid_pin[2] = {
    moonbotPortToPin(LEFT_AVOID_PORT, kPortPin1),
    moonbotPortToPin(LEFT_AVOID_PORT, kPortPin2)
};
uint8_t right_avoid_pin[2] = {
    moonbotPortToPin(RIGHT_AVOID_PORT, kPortPin1),
    moonbotPortToPin(RIGHT_AVOID_PORT, kPortPin2)
};

MoonBotRover rover(Mu);

void setup() {
  Serial.begin(9600);
  Serial3.begin(115200);
  // enable MU on Serial3(Port9)
  Mu.begin(&Serial3);
  // enable TankBase
  TankBase.begin();
  // TankBase RPM correction
  TankBase.rpmCorrection(114);
  // TankBase straight distance correction
  TankBase.distanceCorrection(112);
  // TankBase turning angle correction
  TankBase.wheelSpacingSet(146);
  onBoardLED.begin();
  moonbot_eyes.begin();
  pinMode(left_avoid_pin[0], INPUT_PULLUP);
  pinMode(left_avoid_pin[1], INPUT_PULLUP);
  pinMode(right_avoid_pin[0], INPUT_PULLUP);
  pinMode(right_avoid_pin[1], INPUT_PULLUP);
  // rover enable
  rover.begin();
}

void loop() {
  // Rover obstacle avoidance
  if (digitalRead(left_avoid_pin[0]) == LOW
      || digitalRead(left_avoid_pin[1]) == LOW) {
    // if detected barrier on the left
    int left_speed = TankBase.read(kLeftMotor);
    int right_speed = TankBase.read(kRightMotor);
    if (left_speed != 0 || right_speed != 0) {
      TankBase.write(80, -80);
      do {
        delay(1000);
      } while (digitalRead(left_avoid_pin[0]) == LOW
          || digitalRead(left_avoid_pin[1]) == LOW);
      TankBase.write(left_speed, right_speed);
    }
  }
  if (digitalRead(right_avoid_pin[0]) == LOW
      || digitalRead(right_avoid_pin[1]) == LOW) {
    // if detected barrier on the right
    int left_speed = TankBase.read(kLeftMotor);
    int right_speed = TankBase.read(kRightMotor);
    if (left_speed != 0 || right_speed != 0) {
      TankBase.write(-80, 80);
      do {
        delay(1000);
      } while (digitalRead(right_avoid_pin[0]) == LOW
          || digitalRead(right_avoid_pin[1]) == LOW);
      TankBase.write(left_speed, right_speed);
    }
  }
  // Rover run.
  rover.runTrafficNumber();
}
