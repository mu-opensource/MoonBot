/*
 * moonbot_hardware_version.h
 *
 *  Created on: 2019年3月27日
 *      Author: ysq
 */

#ifndef MOONBOT_MOONBOT_HARDWARE_VERSION_H_
#define MOONBOT_MOONBOT_HARDWARE_VERSION_H_

#define MOOONBOT_HARDWARE_VERSION   100

#if MOOONBOT_HARDWARE_VERSION == 100
// Motor
#define MOONBOT_MOTOR_REDUCTION_RATION    (120)
// Wheel
#define MOONBOT_WHEEL_DIAMETER            (5.0f)    //cm
#define MOONBOT_WHEEL_SPACING             (11.5f)   //cm
// Servo
#define MOONBOT_SERVO_REVERSE             false
// Magnetometer
#define MOONBOT_MECH_MAG_MAIN_AXES        kDirZ
#define MOONBOT_MECH_MAG_SUB_AXES         kDirX
#define MOONBOT_ROVER_MAG_MAIN_AXES       kDirY
#define MOONBOT_ROVER_MAG_SUB_AXES        kDirX
// External WS2812B LED
#define MOONBOT_EXTERNAL_LED_NUM          12
// Vision sensor
#define MU_VISION_SENSOR_DEFAULT_ADDRESS  (0x60)

#endif

#endif /* MOONBOT_MOONBOT_HARDWARE_VERSION_H_ */
