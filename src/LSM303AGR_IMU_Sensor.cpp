/*
 * LSM303AGR_IMU_SENSOR.cpp
 *
 *  Created on: 2019年3月28日
 *      Author: ysq
 */

#include "pins_moonbot.h"
#include "LSM303AGR_IMU_Sensor.h"

LSM303AGR_IMU_Sensor IMU;

LSM303AGR_IMU_Sensor::LSM303AGR_IMU_Sensor(void)
    : lsm_i2c_(MOONBOT_PIN_IMU_SDA, MOONBOT_PIN_IMU_SCL),
      Acc(&lsm_i2c_),
      Mag(&lsm_i2c_) {
}

LSM303AGR_IMU_Sensor::~LSM303AGR_IMU_Sensor(void) {
  // temperature sensor disable
//  LSM303AGR_ACC_W_TEMP_EN_bits(&Acc, LSM303AGR_ACC_TEMP_EN_DISABLED);
  Acc.WriteReg(LSM303AGR_ACC_TEMP_CFG_REG, 0x0);
}

int LSM303AGR_IMU_Sensor::enable(void) {
  int ret = 0;
  pinMode(MOONBOT_PIN_IMU_EN, OUTPUT);
  digitalWrite(MOONBOT_PIN_IMU_EN, HIGH);
  lsm_i2c_.begin();
  delay(10);    //delay for accelerometer sensor initialize
  ret = Acc.Enable();
  if (ret) return ret;
  ret = Mag.Enable();
  if (ret) return ret;
  // temperature sensor enable
//  LSM303AGR_ACC_W_TEMP_EN_bits(&Acc, LSM303AGR_ACC_TEMP_EN_ENABLED);
  ret = Acc.WriteReg(LSM303AGR_ACC_TEMP_CFG_REG, 0xC0);
  return ret;
}

bool LSM303AGR_IMU_Sensor::calibrateMag(void) {
  unsigned long start_time = millis();
  int32_t magnetometer[3] = { 0 };
  for (;;) {
    if (Mag.GetAxes(magnetometer) != LSM303AGR_MAG_STATUS_OK) {
      return false;
    }
    for (int i = 0; i < 3; ++i) {
      if (magnetometer[i] > extremum_[i][1]) {
        extremum_[i][1] = magnetometer[i];
        start_time = millis();
      }
      if (magnetometer[i] < extremum_[i][0]) {
        extremum_[i][0] = magnetometer[i];
        start_time = millis();
      }
      offset_[i] = (extremum_[i][0]+extremum_[i][1])/2;
      if (millis() - start_time > 3000) {
        return true;
      }
    }
  }
  return false;
}

int LSM303AGR_IMU_Sensor::advGetMagAngle(lsm303_axes_t main_axes,
                                         lsm303_axes_t sub_axes) {
  // Read magnetometer LSM303AGR.
  int32_t magnetometer[3] = { 0 };
  if (Mag.GetAxes(magnetometer) != LSM303AGR_MAG_STATUS_OK) {
    return -1;
  }
  int angle = degrees(atan2(magnetometer[sub_axes]-offset_[sub_axes],
                            magnetometer[main_axes]-offset_[main_axes]))+0.5;
  if (angle < 0) {
    angle = 360.0+angle;
  }
  return angle;
}

int LSM303AGR_IMU_Sensor::getMagAngle(void) {
  int32_t accelerometer[3] = { 0 };
  if (Acc.GetAxes(accelerometer) != LSM303AGR_ACC_STATUS_OK) {
    return -1;
  }
  lsm303_axes_t main_axes;
  lsm303_axes_t sub_axes;
  int max_axes_offset = 0;
  int32_t max_axes = int32_t(bit(sizeof(int32_t)*8-1));
  for (int i = 0; i < 3; ++i) {
    if (accelerometer[i]>max_axes) {
      max_axes = accelerometer[i];
      max_axes_offset = i;
    }
  }
  switch (max_axes_offset) {
    case kDirX:
      main_axes = kDirZ;
      sub_axes = kDirY;
      break;
    case kDirY:
      main_axes = kDirZ;
      sub_axes = kDirX;
      break;
    case kDirZ:
      main_axes = kDirY;
      sub_axes = kDirX;
      break;
    default:
      return 0;
  }
  return advGetMagAngle(main_axes, sub_axes);
}

int LSM303AGR_IMU_Sensor::getAccAngle(lsm303_acc_angle_t angle_type) {
  int32_t accelerometer[3] = { 0 };
  if (Acc.GetAxes(accelerometer) != LSM303AGR_ACC_STATUS_OK) {
    return -1;
  }

  int angle = degrees(atan2(accelerometer[kDirX],
                            accelerometer[kDirZ]))+0.5;
  switch (angle_type) {
    case kAccPitch:
      angle = degrees(atan2(accelerometer[kDirY],
              accelerometer[kDirZ])) + 0.5;
      break;
    case kAccRoll:
      angle = degrees(atan2(accelerometer[kDirX],
              accelerometer[kDirZ])) + 0.5;
      break;
    default:
      break;
  }
  return angle;
}

bool LSM303AGR_IMU_Sensor::on(imu_state_t imu_state) {
  switch (imu_state) {
    case kIMUShake: {
      if (getAcceleration() > 1.5) {
        return true;
      }
      return false;
    }
    case kIMUFreeFall: {
      if (getAcceleration() < 0.1) {
        return true;
      }
      return false;
    }
    default:
      break;
  }
  return false;
}

float LSM303AGR_IMU_Sensor::getAcceleration(void) {
  int32_t accelerometer[3] = { 0 };
  if (Acc.GetAxes(accelerometer) != LSM303AGR_ACC_STATUS_OK) {
    return 0;
  }
  int sum_acc = sqrt(accelerometer[0]*accelerometer[0]
                     +accelerometer[1]*accelerometer[1]
                     +accelerometer[2]*accelerometer[2]);
  return sum_acc/1024.0;
}

int16_t LSM303AGR_IMU_Sensor::temperature(void) {
  uint8_t temp[2];
  Acc.ReadReg(LSM303AGR_ACC_OUT_ADC3_L, &temp[0]);
//  if (err) printf("Acc.ReadReg(LSM303AGR_ACC_OUT_ADC3_L err\n");
  Acc.ReadReg(LSM303AGR_ACC_OUT_ADC3_H, &temp[1]);
//  if (err) printf("Acc.ReadReg(LSM303AGR_ACC_OUT_ADC3_H err\n");

  return (temp[1]<<8) | temp[0];
}



