/*
 * MoonBot_Rover.h
 *
 *  Created on: 2019年4月21日
 *      Author: ysq
 */

#ifndef MOONBOT_MOONBOT_ROVER_H_
#define MOONBOT_MOONBOT_ROVER_H_

#include "MuVisionSensor.h"
#include "MoonBot_Servo.h"

extern MuVisionSensor Mu;

class MoonBotRover {
 public:
  MoonBotRover(MuVisionSensor& mu = Mu);
  virtual ~MoonBotRover(void);
  MoonBotRover& operator=(const MoonBotRover &) = delete;
  MoonBotRover(const MoonBotRover&) = delete;

  int begin(void);
  void end(void);
  void runTrafficOnly(void);
  void runTrafficNumber(void);
  int followBallBegin(void);
  void followBallEnd(void);
  void runFollowBall(void);

  uint8_t ball_center_x_ = 50;

 private:
  const int ball_search_rpm_ = 100;
  int ball_x_ = 50;

 protected:
  MuVisionSensor& Mu_;
//  MoonBotServo& head_;
  uint8_t tone_pin_ = MOONBOT_PIN_BUZZER_SIG;
};



#endif /* MOONBOT_MOONBOT_ROVER_H_ */
