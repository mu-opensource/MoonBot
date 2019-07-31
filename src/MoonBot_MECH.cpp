/*
 * MoonBot_MECH.cpp
 *
 *  Created on: 2019年3月20日
 *      Author: ysq
 */

#include <MoonBot_TankBase.h>
#include "MoonBot_MECH.h"

extern MoonBotTankBase TankBase;
extern MuVisionSensor Mu;
extern MoonBotServo m_servo[kServoNum];

MoonBotMECH::MoonBotMECH()
    : MoonBotMECH(Mu, m_servo[kServo4], m_servo[kServo3], m_servo[kServo1]) {
  m_servo[kServo4].attach(kServo4, true);
  m_servo[kServo3].attach(kServo3, true);
  m_servo[kServo1].attach(kServo1, true);
}

MoonBotMECH::MoonBotMECH(MuVisionSensor& Mu,
                         MoonBotServo& claw,
                         MoonBotServo& lower_arm,
                         MoonBotServo& upper_arm)
    : Mu_(&Mu),
      claw_(claw),
      lower_arm_(lower_arm),
      upper_arm_(upper_arm){
  randomSeed(analogRead(0));
  is_time2search_ball_.start(time2search_ball_, AsyncDelay::MILLIS);
  is_time2search_ball_.expire();
  is_time2search_card_.start(time2search_card_, AsyncDelay::MILLIS);
  is_time2search_card_.expire();
}

MoonBotMECH::~MoonBotMECH(void) {
}

int MoonBotMECH::begin(void) {
  Mu_->CameraSetFPS(kFPSHigh);
  return 0;
}

void MoonBotMECH::end(void) {
  is_time2search_ball_.expire();
  is_time2search_card_.expire();
  TankBase.write(0, 0);
  SetVision(0);
}

void MoonBotMECH::setClaw(claw_t claw_type) {
  const int offset = 5;    // 10mm, 1cm
  switch (claw_type) {
    case kClawOpen: {
      claw_.write(150);
    }
      break;
    case kClawClose: {
      claw_.setTargetAngle(claw_close_, 2);
      while(MoonBotServo::moveAllServoToTarget(0)) {
        if (claw_.isPowerOverload()) {
          claw_.stop();
          claw_.write(claw_.read()+5);
          break;
        }
      }
    }
      break;
    case kClawForward: {
      int angle_u, angle_l;
      if (getAngle(claw_x, claw_y, &angle_u, &angle_l)
          || abs(angle_u-upper_arm_.read())>1
          || abs(angle_l-lower_arm_.read())>1) {
        getClawPosition(&claw_x, &claw_y);
      }
      int x = claw_x+offset, y = claw_y;
      if (setClawPosition(x, y)) {
        claw_x = x;
      }
    }
      break;
    case kClawBackward: {
      int angle_u, angle_l;
      if (getAngle(claw_x, claw_y, &angle_u, &angle_l)
          || abs(angle_u-upper_arm_.read())>1
          || abs(angle_l-lower_arm_.read())>1) {
        getClawPosition(&claw_x, &claw_y);
      }
      int x = claw_x-offset, y = claw_y;
      if (setClawPosition(x, y)) {
        claw_x = x;
      }
    }
      break;
    case kClawUp: {
      int angle_u, angle_l;
      if (getAngle(claw_x, claw_y, &angle_u, &angle_l)
          || abs(angle_u-upper_arm_.read())>1
          || abs(angle_l-lower_arm_.read())>1) {
        getClawPosition(&claw_x, &claw_y);
      }
      int x = claw_x, y = claw_y+offset;
      if (setClawPosition(x, y)) {
        claw_y = y;
      }
    }
      break;
    case kClawDown: {
      int angle_u, angle_l;
      if (getAngle(claw_x, claw_y, &angle_u, &angle_l)
          || abs(angle_u-upper_arm_.read())>1
          || abs(angle_l-lower_arm_.read())>1) {
        getClawPosition(&claw_x, &claw_y);
      }
      int x = claw_x, y = claw_y-offset;
      if (setClawPosition(x, y)) {
        claw_y = y;
      }
    }
      break;
    default:
      break;
  }
}

void MoonBotMECH::SetVision(MuVisionType vision_type) {
  if (vision_type_ != vision_type) {
    Mu_->VisionEnd(vision_type_);
    vision_type_ = vision_type;
    switch (vision_type) {
      case VISION_BALL_DETECT:
        Mu_->CameraSetZoom(kZoom1);
        Mu_->VisionBegin(VISION_BALL_DETECT);
        break;
      case VISION_SHAPE_CARD_DETECT:
      case VISION_TRAFFIC_CARD_DETECT:
      case VISION_NUM_CARD_DETECT:
        Mu_->CameraSetZoom(kZoom3);
        Mu_->VisionBegin(vision_type);
        Mu_->VisionSetLevel(vision_type, kLevelBalance);
        break;
      default:
        break;
    }
  }
}

bool MoonBotMECH::UpdateResult(MuVisionType vision_type) {
  if (Mu_->UpdateResult(vision_type, false)&vision_type) {
    switch (vision_type) {
      case VISION_BALL_DETECT:
        if (Mu_->read(vision_type, kStatus)) {
          ball_x_ = Mu_->GetValue(vision_type, kXValue);
        }
        break;
      case VISION_SHAPE_CARD_DETECT:
      case VISION_TRAFFIC_CARD_DETECT:
      case VISION_NUM_CARD_DETECT:
        if (Mu_->read(vision_type, kStatus)) {
          card_x_ = Mu_->GetValue(vision_type, kXValue);
        }
        break;
      default:
        break;
    }
    return true;
  }
  return false;
}

void MoonBotMECH::SetZoom(MuVsCameraZoom zoom) {
  if (zoom_now_ != zoom) {
    zoom_now_ = zoom;
    zoom_count_ = 0;
    Mu_->CameraSetZoom(zoom_now_);
  }
}

bool MoonBotMECH::searchBall(void) {
  claw_.setTargetAngle(claw_open_);
  if (is_time2search_ball_.isExpired()) {
    is_time2search_ball_.repeat();
    switch (random(3)) {
      case 0:
        upper_arm_.setTargetAngle(upper_arm_init_, 2);
        lower_arm_.setTargetAngle(lower_arm_init_, 2);
        MoonBotServo::moveAllServoToTarget();
        break;
      case 1:
        upper_arm_.setTargetAngle(upper_arm_init_+10, 2);
        lower_arm_.setTargetAngle(lower_arm_init_, 2);
        MoonBotServo::moveAllServoToTarget();
        break;
      case 2:
        upper_arm_.setTargetAngle(upper_arm_init_+5, 2);
        lower_arm_.setTargetAngle(lower_arm_init_, 2);
        MoonBotServo::moveAllServoToTarget();
        break;
      default:
        break;
    }
  }
  // set different zoom
//  zoom_count_++;
//  int upper_arm_angle = upper_arm_.read();
//  if (zoom_count_%5 == 4) {
//    if (upper_arm_angle >= upper_arm_init_+10) {
//      if (zoom_now_ == kZoom4) {
//        SetZoom(kZoom2);
//      } else {
//        SetZoom(MuVsCameraZoom(zoom_now_+1));
//      }
//    } if (upper_arm_angle >= upper_arm_init_+5) {
//      if (zoom_now_ == kZoom3) {
//        SetZoom(kZoom1);
//      } else {
//        SetZoom(MuVsCameraZoom(zoom_now_+1));
//      }
//    } else {
//      SetZoom(kZoom1);
//    }
//  }
  SetVision(VISION_BALL_DETECT);
  if (UpdateResult(VISION_BALL_DETECT) == false) {
    return false;
  }
  if (Mu_->read(VISION_BALL_DETECT, kStatus)) {
    TankBase.write(0, 0);
    return true;
  }

  if (ball_x_ > ball_center_x_) {
    TankBase.write(ball_search_rpm_, -ball_search_rpm_);
  } else {
    TankBase.write(-ball_search_rpm_, ball_search_rpm_);
  }
  return false;
}

moonbot_mech_grab_ball_t MoonBotMECH::grabBall(void) {
  static bool is_grab = false;
  is_time2search_ball_.start(time2search_ball_, AsyncDelay::MILLIS);
  SetVision(VISION_BALL_DETECT);
  if (UpdateResult(VISION_BALL_DETECT) == false) {
    return kFollowBall;
  }
  if (Mu_->read(VISION_BALL_DETECT, kStatus) == 0) {
    TankBase.write(0, 0);
    is_grab = false;
    return kUndetectBall;
  }
  if (is_grab) {
    if (Mu_->GetValue(VISION_BALL_DETECT, kXValue) < ball_center_x_ - 5) {
      TankBase.write(-ball_grab_rpm_, ball_grab_rpm_);
    } else if (Mu_->GetValue(VISION_BALL_DETECT, kXValue) > ball_center_x_ + 5) {
      TankBase.write(ball_grab_rpm_, -ball_grab_rpm_);
    } else {
      TankBase.write(ball_grab_rpm_, ball_grab_rpm_);
      if (Mu_->GetValue(VISION_BALL_DETECT, kYValue) >= ball_grab_y_) {
        if (upper_arm_.read() != (int)upper_arm_ready_grab_) {
          upper_arm_.write(upper_arm_ready_grab_);
        }
        TankBase.write(0, 0);
        claw_.setTargetAngle(claw_close_, 2);
        MoonBotServo::moveAllServoToTarget();
        is_grab = false;
        return kGrabedBall;
      }
    }
  } else {
    if (Mu_->GetValue(VISION_BALL_DETECT, kWidthValue) < 26) {
      if (Mu_->GetValue(VISION_BALL_DETECT, kXValue) < ball_center_x_-13) {
        TankBase.write(-ball_search_rpm_, ball_search_rpm_);
      } else if (Mu_->GetValue(VISION_BALL_DETECT, kXValue) > ball_center_x_ + 13) {
        TankBase.write(ball_search_rpm_, -ball_search_rpm_);
      } else {
        TankBase.write(ball_search_rpm_, ball_search_rpm_);
      }
      int angle = upper_arm_.read();
//      printf("y = %d\n", Mu_->GetValue(VISION_BALL_DETECT, kYValue));
      if (Mu_->GetValue(VISION_BALL_DETECT, kYValue) < ball_center_y_-8) {
//        printf("write angle = %d\n", angle + ((ball_center_y_ - Mu_->GetValue(VISION_BALL_DETECT, kYValue))>>4));
//        if ((int)upper_arm_ready_grab_ > angle) {
        upper_arm_.write(angle + ((ball_center_y_ - Mu_->GetValue(VISION_BALL_DETECT, kYValue))>>4));
//        }
      } else if (Mu_->GetValue(VISION_BALL_DETECT, kYValue) > ball_center_y_+8) {
        if (angle > (int)upper_arm_ready_grab_) {
          upper_arm_.write(angle - ((Mu_->GetValue(VISION_BALL_DETECT, kYValue) - ball_center_y_)>>4));
        }
      }
    } else {
      upper_arm_.write(upper_arm_ready_grab_);
      is_grab = true;
    }
  }
  return kFollowBall;
}

bool MoonBotMECH::searchCard(void) {
  SetVision(card_type_);
  if (UpdateResult(card_type_) == false) {
    return false;
  }
  if (Mu_->read(card_type_, kStatus)) {
    TankBase.write(0, 0);
    return true;
  }
  if (is_time2search_card_.isExpired()) {
    is_time2search_card_.repeat();
    switch (random(3)) {
      case 0:
        lower_arm_.setTargetAngle(lower_arm_grabbed_, 2);
        upper_arm_.setTargetAngle(upper_arm_grabbed_, 2);
        MoonBotServo::moveAllServoToTarget();
        break;
      case 1:
        lower_arm_.setTargetAngle(lower_arm_grabbed_, 2);
        upper_arm_.setTargetAngle(upper_arm_grabbed_+3, 2);
        MoonBotServo::moveAllServoToTarget();
        break;
      case 2:
        lower_arm_.setTargetAngle(lower_arm_grabbed_, 2);
        upper_arm_.setTargetAngle(upper_arm_grabbed_+6, 2);
        MoonBotServo::moveAllServoToTarget();
        break;
      default:
        break;
    }
  }

  if (card_x_ > card_center_x_) {
    TankBase.write(ball_search_rpm_, -ball_search_rpm_);
  } else {
    TankBase.write(-ball_search_rpm_, ball_search_rpm_);
  }

  return false;
}

moonbot_mech_shoot_ball_t MoonBotMECH::shootBall(void) {
  SetVision(card_type_);
  if (UpdateResult(card_type_) == false) {
    return kFollowCard;
  }
  if (Mu_->read(card_type_, kStatus) == 0) {
    TankBase.write(0, 0);
    upper_arm_.write(upper_arm_grabbed_);
    return kUndetectCard;
  }
  if (Mu_->GetValue(card_type_, kWidthValue) < shoot_card_width_-4) {
    if (Mu_->GetValue(card_type_, kXValue) < card_center_x_-11) {
      TankBase.write(0, card_follow_rpm_);
    } else if (Mu_->GetValue(card_type_, kXValue) > card_center_x_+11) {
      TankBase.write(card_follow_rpm_, 0);
    } else {
      TankBase.write(card_follow_rpm_, card_follow_rpm_);
    }
    uint16_t angle = upper_arm_.read();
    if (Mu_->GetValue(card_type_, kYValue) < card_center_y_-5) {
      upper_arm_.write(angle + 1);
    } else if (Mu_->GetValue(card_type_, kYValue) > card_center_y_+5){
      upper_arm_.write(angle - 1);
    }
  }
  else if (Mu_->GetValue(card_type_, kWidthValue) > shoot_card_width_+4) {
    // too close, backward
    TankBase.write(-50, -50);
  } else {
    TankBase.write(0, 0);
    switch (random(2)) {
      case 0:
        upper_arm_.setTargetAngle(upper_arm_shoot_, 2);
        MoonBotServo::moveAllServoToTarget();
        lower_arm_.write(lower_arm_shoot_);
        delay(200);
        break;
      case 1:
        upper_arm_.setTargetAngle(upper_arm_shoot_, 2);
        lower_arm_.setTargetAngle(lower_arm_shoot_, 2);
        MoonBotServo::moveAllServoToTarget();
        break;
      default:
        break;
    }
    claw_.setTargetAngle(claw_open_, 2);
    MoonBotServo::moveAllServoToTarget();
    lower_arm_.setTargetAngle(lower_arm_grabbed_, 2);
    upper_arm_.setTargetAngle(upper_arm_grabbed_, 2);
    MoonBotServo::moveAllServoToTarget();
    TankBase.write(-120, -120);
    delay(2000);
    TankBase.write(0, 0);
    upper_arm_.setTargetAngle(upper_arm_init_, 2);
    lower_arm_.setTargetAngle(lower_arm_init_, 2);
    MoonBotServo::moveAllServoToTarget();
//    switch (random(2)) {            // remove action: turn around in 19.06.21
//      case 0:
//        TankBase.write(-150, 150);
//        delay(2000);
//        TankBase.write(0, 0);
//        break;
//      case 1:
//        TankBase.write(150, -150);
//        delay(2000);
//        TankBase.write(0, 0);
//        break;
//      default:
//        break;
//    }
    return kShootedBall;
  }

  return kFollowCard;
}

bool MoonBotMECH::getAngle(int x, int y, int* angle_u, int* angle_l) {
  long k2 = long(x)*x+long(y)*y;
  if (k2 > long(LOWER_ARM_LEN+UPPER_ARM_LEN)*long(LOWER_ARM_LEN+UPPER_ARM_LEN)) return false;
  *angle_u = (
      acos(double(LOWER_ARM_LEN*LOWER_ARM_LEN+UPPER_ARM_LEN*UPPER_ARM_LEN-k2)
      /(2*LOWER_ARM_LEN*UPPER_ARM_LEN))
      )*RAD_TO_DEG-44.5;

  if (x != 0) {
    double angle_l1 = atan(double(y)/x);
    if (angle_l1 < 0 && x < 0) {
      angle_l1 = PI+angle_l1;
    }
    *angle_l = (
        angle_l1
        +acos(double(LOWER_ARM_LEN*LOWER_ARM_LEN+k2-UPPER_ARM_LEN*UPPER_ARM_LEN)
        /(2*LOWER_ARM_LEN*sqrt(k2)))
        )*RAD_TO_DEG+0.5;
  } else {
    *angle_l = (
        PI/2+
        acos(double(LOWER_ARM_LEN*LOWER_ARM_LEN+k2-UPPER_ARM_LEN*UPPER_ARM_LEN)
        /(2*LOWER_ARM_LEN*sqrt(k2)))
        )*RAD_TO_DEG+0.5;
  }
  return true;
}

void MoonBotMECH::getClawPosition(int *x, int *y) {
  int angle_u = upper_arm_.read();
  int angle_l = lower_arm_.read();
  *x = -UPPER_ARM_LEN*cos((angle_l+angle_u+45)*DEG_TO_RAD)+LOWER_ARM_LEN*cos(angle_l*DEG_TO_RAD);
  *y = -UPPER_ARM_LEN*sin((angle_l+angle_u+45)*DEG_TO_RAD)+LOWER_ARM_LEN*sin(angle_l*DEG_TO_RAD);
}

bool MoonBotMECH::setClawPosition(int x, int y) {
  int angle_u = 0, angle_l = 0;
  if (getAngle(x, y, &angle_u, &angle_l) == false) return false;
//  printf("angle_u = %d, angle_l = %d\n", angle_u, angle_l);
  if (angle_u > 180 || angle_u < 20
      || angle_l > 180 || angle_l < 0) {
    return false;
  }
  upper_arm_.write(angle_u);
  lower_arm_.write(angle_l);
  return true;
}


