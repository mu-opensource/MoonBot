/*
 * MoonBot_MECH.h
 *
 *  Created on: 2019年3月20日
 *      Author: ysq
 */

#ifndef MOONBOT_MOONBOT_MECH_H_
#define MOONBOT_MOONBOT_MECH_H_

#include "MuVisionSensor.h"
#include "MoonBot_Servo.h"

#include <SoftwareSerial.h>
#include <AsyncDelay.h>

#define LOWER_ARM_LEN   (60)    // mm
#define UPPER_ARM_LEN   (160)    // mm

enum moonbot_mech_grab_ball_t {
  kGrabedBall,
  kFollowBall,
  kUndetectBall,
};
enum moonbot_mech_shoot_ball_t {
  kShootedBall,
  kFollowCard,
  kUndetectCard,
};

enum claw_t {
  kClawOpen,
  kClawClose,
  kClawForward,
  kClawBackward,
  kClawUp,
  kClawDown,
};

class MoonBotMECH {
 public:
  MoonBotMECH();
  MoonBotMECH(MuVisionSensor& Mu,
              MoonBotServo& claw,
              MoonBotServo& lower_arm,
              MoonBotServo& upper_arm);
  virtual ~MoonBotMECH(void);
  MoonBotMECH& operator=(const MoonBotMECH &) = delete;
  MoonBotMECH(const MoonBotMECH&) = delete;

  int begin(void);
  void end(void);

  void setClaw(claw_t claw_type);
  bool searchBall(void);
  moonbot_mech_grab_ball_t grabBall(void);
  bool searchCard(void);
  moonbot_mech_shoot_ball_t shootBall(void);

  bool getAngle(int x, int y, int* angle_u, int* angle_l);
  void getClawPosition(int *x, int *y);
  bool setClawPosition(int x = UPPER_ARM_LEN, int y = UPPER_ARM_LEN);

  MuVisionType card_type_ = VISION_SHAPE_CARD_DETECT;
  // follow ball x center
  uint8_t ball_center_x_ = 50;
  uint8_t ball_center_y_ = 40;
  // if ball_y >= ball_grab_y_value_, grab ball
  uint8_t ball_grab_y_ = 70;
  // follow card x center
  uint8_t card_center_x_ = 50;
  // shoot ball x center
  uint8_t card_center_y_ = 45;
  uint8_t shoot_card_width_ = 48;

 protected:
  void SetVision(MuVisionType vision_type);
  void UpdateResult(MuVisionType vision_type);

  MuVisionSensor* Mu_;
  MoonBotServo& claw_;
  MoonBotServo& lower_arm_;
  MoonBotServo& upper_arm_;
  AsyncDelay is_time2search_ball_;
  AsyncDelay is_time2search_card_;
  unsigned long time2search_ball_ = 5000;
  unsigned long time2search_card_ = 5000;
  MuVisionType vision_type_ = (MuVisionType)NULL;

  // servo angle init
  uint8_t lower_arm_init_ = 0;
  uint8_t lower_arm_grabbed_ = 120;
  uint8_t upper_arm_grabbed_ = 20;
  // claw open angle
  uint8_t claw_open_ = 120;
  // claw close angle
  uint8_t claw_close_ = 100;
  // Angle of upper servo grab ball
  uint8_t upper_arm_ready_grab_ = 90;
  // Angle of upper servo search ball
  uint8_t upper_arm_init_ = 100;
  uint8_t upper_arm_shoot_ = upper_arm_grabbed_+70;
  uint8_t lower_arm_shoot_ = lower_arm_grabbed_-65;

  const int ball_search_rpm_ = 100;
  const int ball_grab_rpm_ = 80;
  const int card_follow_rpm_ = 100;
  int ball_x_ = 50;
  int card_x_ = 50;

  int claw_x = 0;
  int claw_y = 0;
};



#endif /* MOONBOT_MOONBOT_MECH_H_ */
