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
  // TODO avoid/follow line

 private:
 protected:
  MuVisionSensor& Mu_;
//  MoonBotServo& head_;
  uint8_t tone_pin_ = MOONBOT_PIN_BUZZER_SIG;
};



#endif /* MOONBOT_MOONBOT_ROVER_H_ */
