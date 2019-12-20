/*
 * MoonBot_Servo.cpp
 *
 *  Created on: 2019年3月14日
 *      Author: ysq
 */

#include <Arduino.h>
#include "MoonBot_Servo.h"

MoonBotServo::moonbot_servo_action_t MoonBotServo::mb_servo[kServoNum];
uint8_t MoonBotServo::moonbot_servo_action_t::is_acting = 0;
AsyncDelay MoonBotServo::delay_20ms;

MoonBotServo::MoonBotServo() {
}
MoonBotServo::~MoonBotServo(void) {
  detach();
}

uint8_t MoonBotServo::attach(moonbot_servo_t servo_port, bool reverse) {
  reverse_ = reverse;
  return Servo::attach(attach_port(servo_port));
}

uint8_t MoonBotServo::attach(moonbot_servo_t servo_port, int min, int max, bool reverse) {
  reverse_ = reverse;
  return Servo::attach(attach_port(servo_port), min, max);
}

void MoonBotServo::detach(void) {
  if (servo_port_ != kServoNum
      && mb_servo[servo_port_].servo) {
    mb_servo[servo_port_].servo = NULL;
    servo_port_ = kServoNum;
    power(false);
  }
  Servo::detach();
}

void MoonBotServo::write(int value) {
  value = value+angle_offset_;
  value = constrain(value, 0, 180);
  if (reverse_) {
    value = 180 - value;
  }
  Servo::write(value);
}

int MoonBotServo::read() {
  int ret = Servo::read();
  if (reverse_) {
    ret = 180 - ret;
  }
  ret = ret-angle_offset_;
  return constrain(ret, 0, 180);
}

void MoonBotServo::reverse(bool state) {
  if (reverse_ != state) {
    int angle = this->read();
    reverse_ = state;
    this->write(angle);
  }
}

uint8_t MoonBotServo::attach_port(moonbot_servo_t servo_port) {
  if (servo_port >= kServoNum) {
    return NOT_A_PORT;
  }
  servo_port_ = servo_port;
  mb_servo[servo_port_].servo = this;
  bitClear(moonbot_servo_action_t::is_acting, servo_port);

  uint8_t pin = moonbotServoToPin(servo_port, kSignal);
  pin_shdn_ = moonbotServoToPin(servo_port, kShutDown);
  pin_state_ = moonbotServoToPin(servo_port, kState);
  pinMode(pin_shdn_, OUTPUT);
  pinMode(pin_state_, INPUT);
  power(true);

  return pin;
}

bool MoonBotServo::isPowerOverload(void) {
  if (digitalRead(pin_state_) == LOW) {
    return true;
  }
  return false;
}

void MoonBotServo::setTargetAngle(int angle, unsigned int speed) {
  if (speed == 0) {
    return;
  }
  if (angle == MoonBotServo::read()) return;
  angle += angle_offset_;
  angle = constrain(angle, 0, 180);
  if (reverse_) {
    angle = 180 - angle;
  }
  mb_servo[servo_port_].stop_angle = angle;
  mb_servo[servo_port_].speed = speed;
  bitSet(moonbot_servo_action_t::is_acting, servo_port_);
}

void MoonBotServo::stop(void) {
  bitClear(moonbot_servo_action_t::is_acting, servo_port_);
}

bool MoonBotServo::isMoving(void) {
  return bitRead(moonbot_servo_action_t::is_acting, servo_port_);
}

void MoonBotServo::power(bool on_off) {
  digitalWrite(pin_shdn_, !on_off);
}

bool MoonBotServo::moveAllServoToTarget(unsigned long timeToWait_ms) {
  unsigned long time_start = millis();
  while (moonbot_servo_action_t::is_acting) {
    if (delay_20ms.isExpired()) {
      delay_20ms.start(20, AsyncDelay::MILLIS);
      for (int i = 0; i < kServoNum; ++i) {
        if (mb_servo[i].servo
            && bitRead(moonbot_servo_action_t::is_acting, i)) {
          int angle_now = mb_servo[i].servo->Servo::read();
          int angle_delta = mb_servo[i].stop_angle-angle_now;
          int sign = angle_delta>0 ? 1:-1;
          int abs_angle = abs(angle_delta);
          if (abs_angle>mb_servo[i].speed) {
            abs_angle = mb_servo[i].speed;
          } else {
            bitClear(moonbot_servo_action_t::is_acting, i);
          }
          mb_servo[i].servo->Servo::write(angle_now+sign*abs_angle);
        }
      }
    }
    if (millis()-time_start >= timeToWait_ms) {
      return true;
    }
  }
  return false;
}

void MoonBotServo::stopAllServo(void) {
  moonbot_servo_action_t::is_acting = 0;
}

void MoonBotServo::correction(int angle_offset) {
  angle_offset_ = angle_offset;
}



