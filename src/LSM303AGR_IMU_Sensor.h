/*
 * LSM303AGR_IMU_Sensor.h
 *
 *  Created on: 2019年3月28日
 *      Author: ysq
 */

#ifndef STM32DUINO_LSM303AGR_LSM303AGR_IMU_SENSOR_H_
#define STM32DUINO_LSM303AGR_LSM303AGR_IMU_SENSOR_H_

#include <SoftwareWire.h>
#include "STM32duino_LSM303AGR/LSM303AGR_ACC_Sensor.h"
#include "STM32duino_LSM303AGR/LSM303AGR_MAG_Sensor.h"

enum lsm303_axes_t {
  kDirX,
  kDirY,
  kDirZ,
};
enum lsm303_acc_angle_t {
  kAccRoll,
  kAccPitch,
};
enum imu_state_t {
  kIMUShake,
  kIMUFreeFall,
};
class LSM303AGR_IMU_Sensor {
 protected:
  I2C lsm_i2c_;
 public:
  LSM303AGR_IMU_Sensor(void);
  virtual ~LSM303AGR_IMU_Sensor(void);
  LSM303AGR_IMU_Sensor& operator=(const LSM303AGR_IMU_Sensor &) = delete;
  LSM303AGR_IMU_Sensor(const LSM303AGR_IMU_Sensor&) = delete;

  int enable(void);
/*
 * @brief  Read angle between main_axes and North
 *                      N(0|360)      main_axes
 *         360-angle <= ↑ =>+angle        ↑
 *                  W ← M → E             M → sub_axes
 *                (270) ↓  (90)
 *                      S(180)
 * @param   main_axes: forward axes
 *          sub_axes : axes on the same plane as the main axes
 * @retval  Angle between main_axes and North,
 *          -1 in case of error, success otherwise.
 * @example:
 *   LSM303AGR_IMU_Sensor IMU;
 *   IMU.enable();
 *   while(1) {
 *     int angle = IMU.getMagAngle(kDirY, kDirX);   // Y axes is forward
 *     printf("angle_y_x = %d\n", angle);
 *   }
 */
  int advGetMagAngle(lsm303_axes_t main_axes,
                     lsm303_axes_t sub_axes);
  int getMagAngle(void);
  int getAccAngle(lsm303_acc_angle_t angle_type);
  float getAcceleration(void);
  bool on(imu_state_t imu_state);
  bool calibrateMag(void);
  int16_t temperature(void);

  LSM303AGR_ACC_Sensor Acc;
  LSM303AGR_MAG_Sensor Mag;

 protected:
  // extremum_[X,Y,Z][MIN,MAX]
  int32_t extremum_[3][2] = {
      // max(int32_t)                       min(int32_t)
      {int32_t(~bit(sizeof(int32_t)*8-1)), int32_t(bit(sizeof(int32_t)*8-1))},
      {int32_t(~bit(sizeof(int32_t)*8-1)), int32_t(bit(sizeof(int32_t)*8-1))},
      {int32_t(~bit(sizeof(int32_t)*8-1)), int32_t(bit(sizeof(int32_t)*8-1))},
  };
  // magnetic offset
  int32_t offset_[3] = {0};

};



#endif /* STM32DUINO_LSM303AGR_LSM303AGR_IMU_SENSOR_H_ */
