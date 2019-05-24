/*
 * MoonBot_Servo.h
 *
 *  Created on: 2019年3月14日
 *      Author: ysq
 */

#ifndef MOONBOT_MOONBOT_SERVO_H_
#define MOONBOT_MOONBOT_SERVO_H_

#include <Servo.h>
#include <AsyncDelay.h>

#include "pins_moonbot.h"
#include "Moonbot_Hardware_Version.h"

class MoonBotServo : public Servo {
 public:
  MoonBotServo();
  virtual ~MoonBotServo(void);
  MoonBotServo& operator=(const MoonBotServo &) = delete;
  MoonBotServo(const MoonBotServo&) = delete;

  // Generate compile error when attach(pin, ...) is used
  uint8_t __attribute__ ((error("'attach(pin, ...)' is not supported in moonbot servo driver, please use class 'Servo' instead."))) attach(int pin);                    // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
  uint8_t __attribute__ ((error("'attach(pin, ...)' is not supported in moonbot servo driver, please use class 'Servo' instead."))) attach(int pin, int min, int max);  // as above but also sets min and max values for writes.

  uint8_t attach(moonbot_servo_t servo_port, bool reverse = MOONBOT_SERVO_REVERSE);
  uint8_t attach(moonbot_servo_t servo_port, int min, int max, bool reverse = MOONBOT_SERVO_REVERSE);
  void detach(void);
  void write(int value);
  int read(void);
  void reverse(bool state);

  void setTargetAngle(int angle, unsigned int speed = 1);
  void stop(void);
  void power(bool);
  void correction(int angle_offset);
  bool isMoving(void);
  bool isPowerOverload(void);

  static bool moveAllServoToTarget(unsigned long timeToWait_ms = 0xFFFFFFFF);
  static void stopAllServo(void);

 private:
  uint8_t attach_port(moonbot_servo_t);
  static AsyncDelay delay_20ms;

 protected:
  struct moonbot_servo_action_t {
    static uint8_t is_acting;
    MoonBotServo* servo = NULL;
    uint8_t stop_angle = 0;
    uint8_t speed = 0;            // angle per 20 ms
  };
  static moonbot_servo_action_t mb_servo[];
  int angle_offset_ = 0;
  moonbot_servo_t servo_port_ = kServoNum;
  uint8_t pin_shdn_ = NOT_A_PIN;
  uint8_t pin_state_ = NOT_A_PIN;
  bool reverse_ = MOONBOT_SERVO_REVERSE;
};


#endif /* MOONBOT_MOONBOT_SERVO_H_ */
