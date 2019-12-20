/*
 * MoonBot_Wheel.cpp
 *
 *  Created on: 2019年3月13日
 *      Author: ysq
 */

#include <MoonBot_TankBase.h>

MoonBotTankBase::MoonBotTankBase(Motor& left_motor,
                           Motor& right_motor)
    : left_motor_(left_motor),
      right_motor_(right_motor) {
}

MoonBotTankBase::~MoonBotTankBase(void) {}

int MoonBotTankBase::begin(const bool reverse_dir,
                           const bool enc_enable) {
  int ret;
  if (reverse_dir) {
    ret = begin(true, false, enc_enable);
  } else {
    ret = begin(false, true, enc_enable);
  }
  return ret;
}

int MoonBotTankBase::begin(const bool left_reverse_dir,
                        const bool right_reverse_dir,
                        const bool enc_enable) {
  left_motor_.begin(left_reverse_dir, enc_enable);
  right_motor_.begin(right_reverse_dir, enc_enable);
  return 0;
}

void MoonBotTankBase::write(int left_vol, int right_vol) {
  left_motor_.write(left_vol);
  right_motor_.write(right_vol);
}

int MoonBotTankBase::read(motor_type_t motor_type) {
  switch (motor_type) {
    case kLeftMotor:
      return left_motor_.read();
    case kRightMotor:
      return right_motor_.read();
    default:
      return 0;
  }
}

void MoonBotTankBase::writeStep(int rpm,
                                uint32_t step,
                                motor_type_t motor_type) {
  switch (motor_type) {
    case kLeftMotor:
      left_motor_.writeStep(step, rpm);
      break;
    case kRightMotor:
      right_motor_.writeStep(step, rpm);
      break;
    default:
      left_motor_.writeStep(step, rpm);
      right_motor_.writeStep(step, rpm);
      break;
  }
}

void MoonBotTankBase::writeRpm(int left_rpm, int right_rpm) {
  left_motor_.writeRpm(left_rpm);
  right_motor_.writeRpm(right_rpm);
}
int MoonBotTankBase::readRpm(motor_type_t motor_type) {
  switch (motor_type) {
    case kLeftMotor:
      return left_motor_.readRpm();
    case kRightMotor:
      return right_motor_.readRpm();
    default:
      return 0;
  }
}

void MoonBotTankBase::writeDistance(int rpm, uint32_t distance_cm) {
  left_motor_.writeDistance(rpm, distance_cm);
  right_motor_.writeDistance(rpm, distance_cm);
}

void MoonBotTankBase::writeAngle(int rpm, uint32_t angle) {
  if (rpm == 0) return;
  uint32_t distance_cm = (PI*wheel_spacing_)*angle/360;
  left_motor_.writeDistance(rpm, distance_cm);
  right_motor_.writeDistance(-rpm, distance_cm);
}

uint32_t MoonBotTankBase::readEncoderPulse(motor_type_t motor_type) {
  switch (motor_type) {
    case kLeftMotor:
      return left_motor_.readEncoderPulse();
    case kRightMotor:
      return right_motor_.readEncoderPulse();
    default:
      return 0;
  }
}

void MoonBotTankBase::wheelSpacingSet(int correct, float space_cm) {
  if (space_cm > 0) {
    wheel_spacing_ = space_cm;
  }
  wheel_spacing_ = wheel_spacing_*correct/100.0;
}

void MoonBotTankBase::rpmCorrection(int percent) {
  percent = 100-percent;
  if (percent > 0) {
    int left_offset = percent/2;
    left_motor_.rpmCorrection(100-left_offset);
    right_motor_.rpmCorrection(100-left_offset+percent);
  } else {
    percent = -percent;
    int left_offset = percent/2;
    left_motor_.rpmCorrection(100+left_offset);
    right_motor_.rpmCorrection(100+left_offset-percent);
  }
}

void MoonBotTankBase::distanceCorrection(int percent) {
  if (percent >= 0) {
    left_motor_.distanceCorrection(percent);
    right_motor_.distanceCorrection(percent);
  }
}

