/*
 * Motor.h
 *
 *  Created on: 2018年11月16日
 *      Author: ysq
 */

#ifndef MOONBOT_MOONBOT_MOTOR_H_
#define MOONBOT_MOONBOT_MOTOR_H_

#include <Arduino.h>

#include "pins_moonbot.h"
#include "Moonbot_Hardware_Version.h"

class Motor {
public:
  Motor(moonbot_motor_t motor_type);
  virtual ~Motor();
  Motor(const Motor&) = delete;
  Motor& operator=(const Motor &) = delete;

  int begin(const bool reverse_dir = false,
            const bool enc_enable = true);
  void write(int vol);
  int read(void);
  /*
   * enc_enable must be set as <true> in function <int begin(reverse_dir, enc_enable)> for
   * <write_step()>&&<write_rev()>&&<write_distance()> to be available.
   */
  void writeStep(uint32_t step, int rpm = 30);
  /*
   * @brief: set motor RPM(Revolutions Per Minute) value with encoder
   * @param: rpm: RPM value
   * @retval: none
   */
  void writeRpm(int rpm = 30);
  /*
   * @brief: read motor RPM(Revolutions Per Minute) value
   * @param: none
   * @retval: rpm value now
   */
  int readRpm(void);
  void writeDistance(int rpm, uint32_t distance_cm);
  uint32_t readEncoderPulse(void) {return enc_count_;}

  void rpmCorrection(uint8_t);
  void distanceCorrection(uint8_t);

  // TODO test fun delete.
//  void test_int_func(void);

private:
  void SetSpeed(uint8_t vol, uint8_t dir);

  // IRQ function
  friend void motor1Interrupt(void);
  friend void motor2Interrupt(void);
  void motorIrq(void);
  inline void stepEvent(void);
  inline void rpmEvent(void);

  bool en_step_event_ = false;
  bool en_rpm_event_ = false;

  const moonbot_motor_t motor_type_;
  uint8_t rpm_correction_ = 100;
  uint8_t distance_correction_ = 100;
  bool enc_enable_ = false;
  uint8_t distance_per_circle_ = 0;
  uint8_t speed_vol_ = 0;
  uint32_t enc_count_ = 0;
  uint32_t speed_rev_ = 0;
  uint32_t current_rev_ = 0;
  uint32_t stop_enc_count_ = 0;
  unsigned long time_start_ = 0;
  uint8_t pin_speed_ = NOT_A_PIN;
  uint8_t pin_dir_ = NOT_A_PIN;
  uint8_t pin_enc_ = NOT_A_PIN;
  uint8_t reverse_dir_ = 0;
};

extern Motor Motor1;
extern Motor Motor2;

#endif /* MOONBOT_MOONBOT_MOTOR_H_ */
