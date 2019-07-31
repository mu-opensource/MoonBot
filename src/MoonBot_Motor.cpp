/*
 * Motor.cpp
 *
 *  Created on: 2018年11月16日
 *      Author: ysq
 */

#include <MoonBot_Motor.h>

Motor Motor1(kMotor1);
Motor Motor2(kMotor2);

void motor1Interrupt(void) {
  Motor1.motorIrq();
}
void motor2Interrupt(void) {
  Motor2.motorIrq();
}

Motor::Motor(moonbot_motor_t motor_type)
    : motor_type_(motor_type) {
}

Motor::~Motor() {
  SetSpeed(0, 0xFF);
}

void Motor::SetSpeed(uint8_t vol, uint8_t dir) {
  speed_vol_ = vol;
  analogWrite(pin_speed_, vol);
  if (dir > 0x1) return;
  digitalWrite(pin_dir_, dir);
}

int Motor::begin(const bool reverse_dir,
                 const bool enc_enable) {
  reverse_dir_ = reverse_dir ? LOW:HIGH;
  pin_speed_ = moonbotMotorToPin(motor_type_, kSpeed);
  pin_dir_ = moonbotMotorToPin(motor_type_, kDirection);
  pinMode(pin_speed_, OUTPUT);
  pinMode(pin_dir_, OUTPUT);
  digitalWrite(pin_speed_, LOW);
   if (enc_enable_ != enc_enable) {
    enc_enable_ = enc_enable;
    if (enc_enable) {
      switch (motor_type_) {
        case kMotor1:
          pin_enc_ = moonbotPortToPin(kPort4, kPortPin1);
          pinMode(pin_enc_, INPUT);
          attachInterrupt(digitalPinToInterrupt(pin_enc_), motor1Interrupt, FALLING);
          break;
        case kMotor2:
          pin_enc_ = moonbotPortToPin(kPort6, kPortPin1);
          pinMode(pin_enc_, INPUT);
          attachInterrupt(digitalPinToInterrupt(pin_enc_), motor2Interrupt, FALLING);
          break;
        default:
          return -1;
      }
    } else {
      switch (motor_type_) {
        case kMotor1:
          detachInterrupt(digitalPinToInterrupt(pin_enc_));
          break;
        case kMotor2:
          detachInterrupt(digitalPinToInterrupt(pin_enc_));
          break;
        default:
          return -1;
      }
    }
  }
  return 0;
}
void Motor::rpmCorrection(uint8_t percent) {
  rpm_correction_ = percent;
}

void Motor::distanceCorrection(uint8_t percent) {
  distance_correction_ = percent;
}

void Motor::write(int vol) {
  en_rpm_event_ = false;
  if (vol > 0) {
    SetSpeed(vol, reverse_dir_);
  } else if (vol < 0) {
    SetSpeed(-vol, !reverse_dir_);
  } else {
    SetSpeed(0, 0xFF);
  }
}

void Motor::writeStep(uint32_t step, int rpm) {
  if (step == 0) {
    write(0);
    return;
  }
  stop_enc_count_ = enc_count_+step*(uint32_t)rpm_correction_/100;
  en_step_event_ = true;
  writeRpm(rpm);
}
void Motor::stepEvent(void) {
  if (en_step_event_ && enc_count_>=stop_enc_count_) {
    en_step_event_ = false;
    write(0);
  }
}

void Motor::writeRpm(int rpm) {
  if (rpm == 0) {
    write(0);
    return;
  }
  if (read() == 0 || (rpm>0) != (read()>0)) {
    if (rpm != 0) {
      write(constrain(rpm*3, -255, 255));
    }
  }
  time_start_ = micros();
  // wheel rev per minute => motor rev per second
  speed_rev_ = abs(rpm*2*MOONBOT_MOTOR_REDUCTION_RATION/60)*rpm_correction_/100;
  en_rpm_event_ = true;
}
void Motor::rpmEvent(void) {
  // XXX there may be other ways to run faster
  // 5477 ms per 10W time
//  if (en_per_sec_event_ ) {
//    unsigned long time_now = micros();
//    uint32_t current_enc_count = enc_count_;
//    uint32_t current_per_sec = (current_enc_count-start_enc_count_)*1000000/(time_now-time_start_);
//    int sign = vol_>0 ? 1:-1;
//    int abs_vol = abs(vol_);
//    if (current_per_sec > speed_per_sec_ && abs_vol) {
//      abs_vol--;
//      write(abs_vol*sign);
//    } else if (current_per_sec < speed_per_sec_ && abs_vol<255) {
//      abs_vol++;
//      write(abs_vol*sign);
//    }
//    time_start_ = time_now;
//    start_enc_count_ = current_enc_count;
//  }
  // 4795 ms per 10W time

  unsigned long time_now = micros();
  // rpm = 1000000us * 60s/min / reduction_ration / time_pass / 2
  current_rev_ = ((uint32_t)1000000)/(time_now-time_start_);
  if (en_rpm_event_) {
//    unsigned long time_now = micros();
//    // rpm = 1000000us * 60s/min / reduction_ration / time_pass / 2
//    current_rev_ = ((uint32_t)1000000)/(time_now-time_start_);
    uint8_t vol = speed_vol_;
    // FIXME calculate time per pulse in function `writeRpm`
    // and compare time here, it mast faster than it is now
    if (current_rev_ > speed_rev_ && vol) {
      vol -= 1;
      SetSpeed(vol, 0xFF);
    } else if (current_rev_ < speed_rev_ && vol<255) {
      vol += 1;
      SetSpeed(vol, 0xFF);
    }
  }
  time_start_ = time_now;
}

void Motor::writeDistance(int rpm, uint32_t distance_cm) {
  uint32_t step = (uint32_t)distance_correction_
      *(MOONBOT_MOTOR_REDUCTION_RATION*2)
      *distance_cm/(MOONBOT_WHEEL_DIAMETER*PI)
      /100;    // REDUCTION_RATIO*2*C/(d*pi)s
  writeStep(step, rpm);
}

int Motor::read(void) {
  int dir = digitalRead(pin_dir_)==HIGH ? 1:-1;
  return dir * speed_vol_ * (reverse_dir_ ? 1:-1);
}

int Motor::readRpm(void) {
  int reverse = reverse_dir_ ? -1:1;
  int sign = digitalRead(pin_dir_)==HIGH ? -reverse:reverse;
  return sign*(int32_t)current_rev_*60*100/(2*MOONBOT_MOTOR_REDUCTION_RATION)/(int32_t)rpm_correction_;
}

void Motor::motorIrq(void) {
  enc_count_++;
  stepEvent();
  rpmEvent();
}

//void Motor::test_int_func(void) {
//  // rev event
//  printf("test per_sec_event() start!\n");
//  en_rpm_event_ = true;
//  unsigned long start_time = millis();
//  for (uint32_t i = 0; i < 100000; ++i) {
//    rpmEvent();
//  }
//  en_rpm_event_ = false;
//  printf("per_sec_event 10W time: %lu ms\n", (millis()-start_time));
//  while(1);
//}








