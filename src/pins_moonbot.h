/*
 * pins_moonbot.h
 *
 *  Created on: 2019年2月26日
 *      Author: ysq
 */

#ifndef MOONBOT_PINS_MOONBOT_H_
#define MOONBOT_PINS_MOONBOT_H_

#include <pins_arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

enum moonbot_servo_t {
  kServo1,
  kServo2,
  kServo3,
  kServo4,
  kServoNum
};

enum moonbot_motor_t {
  kMotor1,
  kMotor2,
  kMotorNum
};

enum moonbot_port_t {
  kPort1,
  kPort2,
  kPort3,
  kPort4,
  kPort5,
  kPort6,
  kPort7,
  kPort8,
  kPort9,
  kPortNum
};

enum servo_pin_t {
  kSignal,
  kShutDown,
  kState,
};
enum port_pin_t {
  kPortPin1,
  kPortPin2,
  kPortPinNum,
};
enum motor_pin_t {
  kDirection,
  kSpeed,
};

#ifdef __AVR_ATmega1280__

// MoonBot Pin Mapping
// servo1
#define MOONBOT_PIN_SERVO1              (5)
#define MOONBOT_PIN_SERVO1_SHDN         (27)
#define MOONBOT_PIN_SERVO1_STATE        (A8)
// servo2
#define MOONBOT_PIN_SERVO2              (47)
#define MOONBOT_PIN_SERVO2_SHDN         (35)
#define MOONBOT_PIN_SERVO2_STATE        (A12)
// servo3
#define MOONBOT_PIN_SERVO3              (38)
#define MOONBOT_PIN_SERVO3_SHDN         (36)
#define MOONBOT_PIN_SERVO3_STATE        (A13)
// servo4
#define MOONBOT_PIN_SERVO4              (22)
#define MOONBOT_PIN_SERVO4_SHDN         (24)
#define MOONBOT_PIN_SERVO4_STATE        (A11)

// motor1
#define MOONBOT_PIN_MOTOR1_DIR          (32)
#define MOONBOT_PIN_MOTOR1_SPD          (8)
// motor2
#define MOONBOT_PIN_MOTOR2_DIR          (37)
#define MOONBOT_PIN_MOTOR2_SPD          (7)

// BSP
// IMU
#define MOONBOT_PIN_IMU_SDA             (34)
#define MOONBOT_PIN_IMU_SCL             (33)
#define MOONBOT_PIN_IMU_EN              (28)
// led
#define MOONBOT_PIN_LED                 (6)
// buzzer
#define MOONBOT_PIN_BUZZER_SIG          (4)
#define MOONBOT_PIN_BUZZER_SHDW         (29)
// button
#define MOONBOT_PIN_BUTTON_A            (A9)
#define MOONBOT_PIN_BUTTON_B            (A14)

#define MOONBOT_PORT_SERIAL1            kPort7
#define MOONBOT_PORT_SERIAL2            kPort2
#define MOONBOT_PORT_SERIAL3            kPort9
#define MOONBOT_PORT_I2C                kPort5

const uint8_t PROGMEM moonbot_port[][2] = {
    //S1    S2
    //RX    TX
    //SCL   SDA
    { 12,   A3 },             //P1
    { 17,   16 },             //P2
    { 11,   A4 },             //P3
    { 3,    A5 },             //P4
    { SCL,  SDA },            //P5
    { 2,    A6 },             //P6
    { 19,   18 },             //P7
    { 13,   A7 },             //P8
    { 15,   14 },             //P9
};
const uint8_t PROGMEM moonbot_servo[][3] = {
    { MOONBOT_PIN_SERVO1,   MOONBOT_PIN_SERVO1_SHDN,    MOONBOT_PIN_SERVO1_STATE},
    { MOONBOT_PIN_SERVO2,   MOONBOT_PIN_SERVO2_SHDN,    MOONBOT_PIN_SERVO2_STATE},
    { MOONBOT_PIN_SERVO3,   MOONBOT_PIN_SERVO3_SHDN,    MOONBOT_PIN_SERVO3_STATE},
    { MOONBOT_PIN_SERVO4,   MOONBOT_PIN_SERVO4_SHDN,    MOONBOT_PIN_SERVO4_STATE},
};
const uint8_t PROGMEM moonbot_motor[][2] = {
    { MOONBOT_PIN_MOTOR1_DIR,    MOONBOT_PIN_MOTOR1_SPD },
    { MOONBOT_PIN_MOTOR2_DIR,    MOONBOT_PIN_MOTOR2_SPD },
};

#endif  //#ifdef __AVR_ATmega1280__

uint8_t moonbotPortToPin(moonbot_port_t port_num, port_pin_t pin_num);
uint8_t moonbotMotorToPin(moonbot_motor_t motor_num, motor_pin_t pin_type);
uint8_t moonbotServoToPin(moonbot_servo_t servo_num, servo_pin_t pin_type);


#ifdef __cplusplus
}
#endif

#endif /* MOONBOT_PINS_MOONBOT_H_ */
