/*
 * MoonBot_Bot.cpp
 *
 *  Created on: 2019年5月20日
 *      Author: ysq
 */

#include <MoonBot_Humannoid.h>

extern MoonBotTankBase TankBase;
extern MuVisionSensor Mu;
extern MoonBotServo m_servo[kServoNum];

MoonBotHumannoid::MoonBotHumannoid()
    : Mu_(Mu),
      head_(m_servo[kServo3]),
      left_arm_(m_servo[kServo4]),
      right_arm_(m_servo[kServo1]) {
  head_.attach(kServo3, true);
  left_arm_.attach(kServo4);
  right_arm_.attach(kServo1, true);
}

MoonBotHumannoid::MoonBotHumannoid(MuVisionSensor& Mu,
             MoonBotServo& head,
             MoonBotServo& left_arm,
             MoonBotServo& right_arm)
    : Mu_(Mu),
      head_(head),
      left_arm_(left_arm),
      right_arm_(right_arm) {
}

MoonBotHumannoid::~MoonBotHumannoid(void) {}

void MoonBotHumannoid::armShake(moonbot_humannoid_arm_t arm_type,
                                uint8_t shake_offset,
                                unsigned long wait) {
  int rarm_angle = right_arm_.read();
  int larm_angle = left_arm_.read();
  if (arm_type != kMoonBotRightArm) {
    left_arm_.write(larm_angle+shake_offset);
  }
  if (arm_type != kMoonBotLeftArm) {
    right_arm_.write(rarm_angle+shake_offset);
  }
  delay(wait);
  if (arm_type != kMoonBotRightArm) {
    left_arm_.write(larm_angle);
  }
  if (arm_type != kMoonBotLeftArm) {
    right_arm_.write(rarm_angle);
  }
  delay(wait);
}

void MoonBotHumannoid::swing(moonbot_humannoid_motor_t motor_type,
                             int speed,
                             unsigned long wait) {
  int head_angle = head_.read();
  int wheel_speed;
  do {
    wheel_speed = motor_type==kMoonBotRightMotor ? TankBase.read(kRightMotor):TankBase.read(kLeftMotor);
    if (wheel_speed != speed) {
      wheel_speed>speed ? wheel_speed--:wheel_speed++;
    }
    switch (motor_type) {
      case kMoonBotRightMotor:
        TankBase.write(0, wheel_speed);
        break;
      case kMoonBotLeftMotor:
        TankBase.write(wheel_speed, 0);
        break;
      case kMoonBotBothMotor:
        TankBase.write(wheel_speed, wheel_speed);
        break;
      default:
        return;
    }
    delay(1);
  } while (wheel_speed!=speed);
  head_.write(head_angle+15);
  delay(wait);
  do {
    wheel_speed = motor_type==kMoonBotRightMotor ? TankBase.read(kRightMotor):TankBase.read(kLeftMotor);
    if (wheel_speed != -speed*2/3) {
      wheel_speed>-speed*2/3 ? wheel_speed--:wheel_speed++;
    }
    switch (motor_type) {
      case kMoonBotRightMotor:
        TankBase.write(0, wheel_speed);
        break;
      case kMoonBotLeftMotor:
        TankBase.write(wheel_speed, 0);
        break;
      case kMoonBotBothMotor:
        TankBase.write(wheel_speed, wheel_speed);
        break;
      default:
        return;
    }
    delay(1);
  } while (wheel_speed!=speed);
  head_.write(head_angle);
  delay(wait*3);
  TankBase.write(0, 0);
}

void MoonBotHumannoid::bodyShake(int speed, unsigned long wait) {
  TankBase.write(speed, -speed);
  delay(wait);
  TankBase.write(-speed, speed);
  delay(wait);
  TankBase.write(0, 0);
}

void MoonBotHumannoid::bodyTwist(int speed,
                                 unsigned long wait) {
  wait = wait<200 ? 200:wait;
  TankBase.write(speed, speed/4);
  delay(200);
  TankBase.write(0, 0);
  delay(wait-200);
  TankBase.write(speed/4, speed);
  delay(200);
  TankBase.write(0, 0);
  delay(wait-200);
}





