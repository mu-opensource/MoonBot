/*
 * MoonBot_Bot.h
 *
 *  Created on: 2019年5月20日
 *      Author: ysq
 */

#ifndef MOONBOT_MOONBOT_BOT_H_
#define MOONBOT_MOONBOT_BOT_H_

#include <MoonBot_TankBase.h>
#include "MuVisionSensor.h"
#include "MoonBot_Servo.h"
#include "MoonBot_Eyes.h"

enum moonbot_humannoid_arm_t {
  kMoonBotLeftArm,
  kMoonBotRightArm,
  kMoonBotBothArm
};
enum moonbot_humannoid_motor_t {
  kMoonBotLeftMotor,
  kMoonBotRightMotor,
  kMoonBotBothMotor
};
class MoonBotHumannoid {
 public:
  MoonBotHumannoid();
  MoonBotHumannoid(MuVisionSensor& Mu,
                   MoonBotServo& head,
                   MoonBotServo& left_arm,
                   MoonBotServo& right_arm);
  virtual ~MoonBotHumannoid(void);
  MoonBotHumannoid& operator=(const MoonBotHumannoid &) = delete;
  MoonBotHumannoid(const MoonBotHumannoid&) = delete;

  // suggest: wait time =  nod_offset
  void nod(uint8_t nod_offset = 15,
           unsigned long wait = 15);
  void armShake(moonbot_humannoid_arm_t arm_type,
                uint8_t shake_offset,
                unsigned long wait);
  void swing(moonbot_humannoid_motor_t motor_type,
             int speed,
             unsigned long wait);
  void bodyShake(int speed,
                 unsigned long wait);
  void bodyTwist(int speed,
                 unsigned long wait);

 private:
  MuVisionSensor& Mu_;
  MoonBotServo& head_;
  MoonBotServo& left_arm_;
  MoonBotServo& right_arm_;
};


#endif /* MOONBOT_MOONBOT_BOT_H_ */
