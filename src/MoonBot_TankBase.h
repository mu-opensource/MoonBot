/*
 * MoonBot_Wheel.h
 *
 *  Created on: 2019年3月13日
 *      Author: ysq
 */

#ifndef MOONBOT_MOONBOT_WHEEL_H_
#define MOONBOT_MOONBOT_WHEEL_H_

#include <Moonbot_Hardware_Version.h>
#include <MoonBot_Motor.h>

enum motor_type_t {
  kLeftMotor,
  kRightMotor
};

class MoonBotTankBase {
 public:
  MoonBotTankBase(Motor& left_motor, Motor& right_motor);
  virtual ~MoonBotTankBase(void);
  MoonBotTankBase& operator=(const MoonBotTankBase &) = delete;
  MoonBotTankBase(const MoonBotTankBase&) = delete;

  int begin(const bool reverse_dir = false,
            const bool enc_enable = true);
  int begin(const bool left_reverse_dir,
            const bool right_reverse_dir,
            const bool enc_enable);
  void write(int left_vol, int right_vol);
  int read(motor_type_t motor_type);
  /*
   * enc_enable must be set as <true> in function
   * <int begin(left_reverse_dir, right_reverse_dir, enc_enable)> for
   * <writeStep()> && <writeRpm()> && <writeDistance()> to be available.
   */
  void forward(unsigned int step, unsigned int rpm = 30) {
    writeDistance(rpm, step);
  }
  void backward(unsigned int step, unsigned int rpm = 30) {
    writeDistance(-rpm, step);
  }
  void turnLeft(unsigned int step, unsigned int rpm = 30) {
    writeAngle(-rpm, step);
  }
  void turnRight(unsigned int step, unsigned int rpm = 30) {
    writeAngle(rpm, step);
  }
  void stop(void) {write(0, 0);}

  void writeStep(int rpm, uint32_t step,
                 motor_type_t motor_type = (motor_type_t)0xFF);
  uint32_t readEncoderPulse(motor_type_t motor_type);
  void writeRpm(int left_rpm, int right_rpm);
  int readRpm(motor_type_t motor_type);
  void writeDistance(int rpm, uint32_t distance_cm);
  void writeAngle(int rpm, uint32_t angle);

  void wheelSpacingSet(int correct, float space_cm = 0);
  void rpmCorrection(int percent);
  void distanceCorrection(int percent);

 private:
  inline bool motorDirectionReverseCheck(int left_speed, int right_speed);

  unsigned int distance_step_ms_ = 200;
  unsigned int angle_step_ms_ = 300;
  Motor& left_motor_;
  Motor& right_motor_;
  float wheel_spacing_ = MOONBOT_WHEEL_SPACING;
};


#endif /* MOONBOT_MOONBOT_WHEEL_H_ */
