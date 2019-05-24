/*
 * MoonBot_MECH.cpp
 *
 *  Created on: 2019年3月20日
 *      Author: ysq
 */

#include <MoonBot_TankBase.h>
#include "MoonBot_MECH.h"

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
  Mu_->CameraSetAwb(kLockWhiteBalance);
  return 0;
}

void MoonBotMECH::end(void) {
  is_time2search_ball_.expire();
  is_time2search_card_.expire();
  TankBase.write(0, 0);
  SetVision(0);
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

void MoonBotMECH::UpdateResult(MuVisionType vision_type) {
  Mu_->GetValue(vision_type, kStatus);
  switch (vision_type) {
    case VISION_BALL_DETECT:
      ball_x_ = Mu_->GetValue(vision_type, kXValue);
      break;
    case VISION_SHAPE_CARD_DETECT:
    case VISION_TRAFFIC_CARD_DETECT:
    case VISION_NUM_CARD_DETECT:
      card_x_ = Mu_->GetValue(vision_type, kXValue);
      break;
    default:
      break;
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
        upper_arm_.setTargetAngle(upper_arm_init_-10, 2);
        lower_arm_.setTargetAngle(lower_arm_init_, 2);
        MoonBotServo::moveAllServoToTarget();
        break;
      case 2:
        upper_arm_.setTargetAngle(upper_arm_init_-5, 2);
        lower_arm_.setTargetAngle(lower_arm_init_, 2);
        MoonBotServo::moveAllServoToTarget();
        break;
      default:
        break;
    }
  }
  SetVision(VISION_BALL_DETECT);
  UpdateResult(VISION_BALL_DETECT);
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
  UpdateResult(VISION_BALL_DETECT);
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
      if (Mu_->GetValue(VISION_BALL_DETECT, kYValue) < ball_center_y_-8) {
//        printf("write angle = %d\n", angle - ((50 - Mu_->GetValue(VISION_BALL_DETECT, kYValue)) >> 4));
        if (angle > 0) {
          upper_arm_.write(angle - ((ball_center_y_ - Mu_->GetValue(VISION_BALL_DETECT, kYValue))>>4));
        }
      } else if (Mu_->GetValue(VISION_BALL_DETECT, kYValue) > ball_center_y_+8) {
        if ((int)upper_arm_ready_grab_ > angle) {
          upper_arm_.write(angle + ((Mu_->GetValue(VISION_BALL_DETECT, kYValue) - ball_center_y_)>>4));
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
  UpdateResult(card_type_);
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
        upper_arm_.setTargetAngle(upper_arm_grabbed_-3, 2);
        MoonBotServo::moveAllServoToTarget();
        break;
      case 2:
        lower_arm_.setTargetAngle(lower_arm_grabbed_, 2);
        upper_arm_.setTargetAngle(upper_arm_grabbed_-6, 2);
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
  UpdateResult(card_type_);
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
      upper_arm_.write(angle - 1);
    } else if (Mu_->GetValue(card_type_, kYValue) > card_center_y_+5){
      upper_arm_.write(angle + 1);
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
    TankBase.write(-150, -150);
    delay(2000);
    TankBase.write(0, 0);
    upper_arm_.setTargetAngle(upper_arm_init_, 2);
    lower_arm_.setTargetAngle(lower_arm_init_, 2);
    MoonBotServo::moveAllServoToTarget();
    switch (random(2)) {
      case 0:
        TankBase.write(-150, 150);
        delay(2000);
        TankBase.write(0, 0);
        break;
      case 1:
        TankBase.write(150, -150);
        delay(2000);
        TankBase.write(0, 0);
        break;
      default:
        break;
    }
    return kShootedBall;
  }

  return kFollowCard;
}




