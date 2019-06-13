/*
 * common.cpp
 *
 *  Created on: 2019年3月20日
 *      Author: ysq
 */

#include "MoonBot.h"

MuVisionSensor Mu(MU_VISION_SENSOR_DEFAULT_ADDRESS);
WT2003S speaker;
LSM303AGR_IMU_Sensor IMU;
MoonBotTankBase TankBase(Motor2, Motor1);
Adafruit_NeoPixel onBoardLED(2, MOONBOT_PIN_LED, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel moonbot_eyes(MOONBOT_EXTERNAL_LED_NUM, moonbotPortToPin(kPort1, kPortPin1), NEO_GRB + NEO_KHZ800);
MoonBotServo m_servo[kServoNum];

uint8_t moonbotPortToPin(moonbot_port_t port_num, port_pin_t pin_num) {
  return pgm_read_byte(&moonbot_port[port_num][pin_num]);
}

uint8_t moonbotMotorToPin(moonbot_motor_t motor_num, motor_pin_t pin_type) {
  return pgm_read_byte(&moonbot_motor[motor_num][pin_type]);
}

uint8_t moonbotServoToPin(moonbot_servo_t servo_num, servo_pin_t pin_type) {
  return pgm_read_byte(&moonbot_servo[servo_num][pin_type]);
}

static uint32_t Wheel_(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return LED.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return LED.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return LED.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
static void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<LED.numPixels(); i++) {
      LED.setPixelColor(i, Wheel_((i+j) & 255));
    }
    LED.show();
    delay(wait);
  }
}
void moonbotInit(int start_button) {
  pinMode(MOONBOT_PIN_BUTTON_A, INPUT);
  pinMode(MOONBOT_PIN_BUTTON_B, INPUT);
  pinMode(MOONBOT_PIN_BUZZER_SIG, OUTPUT);
  pinMode(MOONBOT_PIN_BUZZER_SHDW, OUTPUT);
  digitalWrite(MOONBOT_PIN_BUZZER_SHDW, LOW);
  LED.begin();
  // disable servo power
  for (int i = 0; i < kServoNum; ++i) {
    int pin = moonbotServoToPin((moonbot_servo_t)i, kShutDown);
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
  for (int i = 0; i < kMotorNum; ++i) {
    int pin = moonbotMotorToPin((moonbot_motor_t)i, kSpeed);
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  // LED and buzzer action
  tone(MOONBOT_PIN_BUZZER_SIG, 500);
  delay(200);
  tone(MOONBOT_PIN_BUZZER_SIG, 1000, 400);
  rainbow(5);
  for (uint8_t i = LED.getBrightness(); i > 0; --i) {
    LED.setBrightness(i);
    LED.show();
    delay(5);
  }
  LED.clear();
  LED.show();
  LED.setBrightness(255);

  if (start_button != NOT_A_PIN) {
    while(digitalRead(start_button) == HIGH);
  }
  digitalWrite(MOONBOT_PIN_BUZZER_SHDW, HIGH);
}

bool isButtonPressed(int key_num) {
  if (digitalRead(key_num) == LOW) {
    while(digitalRead(key_num) == HIGH);
    return true;
  }
  return false;
}
