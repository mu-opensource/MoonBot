/*
 * MoonBot_Rover.cpp
 *
 *  Created on: 2019年4月21日
 *      Author: ysq
 */
#include <MoonBot_Rover.h>
#include <Adafruit_NeoPixel.h>
#include <MoonBot_TankBase.h>

extern Adafruit_NeoPixel onBoardLED;
extern MoonBotTankBase TankBase;

MoonBotRover::MoonBotRover(MuVisionSensor& mu)
    : Mu_(mu) {
}

MoonBotRover::~MoonBotRover(void) {
}

int MoonBotRover::begin(void) {
//  head_.write(100);
  Mu_.VisionBegin(VISION_TRAFFIC_CARD_DETECT);
  Mu_.VisionBegin(VISION_NUM_CARD_DETECT);
  Mu_.VisionSetLevel(VISION_TRAFFIC_CARD_DETECT, kLevelAccuracy);
  Mu_.VisionSetLevel(VISION_NUM_CARD_DETECT, kLevelAccuracy);
  return 0;
}

void MoonBotRover::end(void) {
  Mu_.VisionEnd(VISION_TRAFFIC_CARD_DETECT);
  Mu_.VisionEnd(VISION_NUM_CARD_DETECT);
  TankBase.write(0, 0);
}

void MoonBotRover::runTrafficOnly(void) {
  if ((Mu_.UpdateResult(VISION_TRAFFIC_CARD_DETECT, false)&VISION_TRAFFIC_CARD_DETECT)
      && Mu_.read(VISION_TRAFFIC_CARD_DETECT, kStatus)) {
    switch (Mu_.read(VISION_TRAFFIC_CARD_DETECT, kLabel)) {
      case MU_TRAFFIC_CARD_FORWARD:
        TankBase.writeRpm(30, 30);
        break;
      case MU_TRAFFIC_CARD_LEFT:
        TankBase.writeAngle(-30, 90);
        break;
      case MU_TRAFFIC_CARD_RIGHT:
        TankBase.writeAngle(30, 90);
        break;
      case MU_TRAFFIC_CARD_TURN_AROUND:
        TankBase.writeAngle(30, 180);
        break;
      case MU_TRAFFIC_CARD_PARK:
        TankBase.write(0, 0);
        break;
      default:
        break;
    }
  }
}

void MoonBotRover::runTrafficNumber(void) {
  if ((Mu_.UpdateResult(VISION_TRAFFIC_CARD_DETECT, false)&VISION_TRAFFIC_CARD_DETECT)
      && Mu_.read(VISION_TRAFFIC_CARD_DETECT, kStatus)) {
    int traffic_label = Mu_.GetValue(VISION_TRAFFIC_CARD_DETECT, kLabel);
//    if (traffic_label == MU_TRAFFIC_CARD_PARK
//        && (Wheel.read(kLeftMotor) == 0)
//        && (Wheel.read(kRightMotor) == 0)) return;
    tone(tone_pin_, 500, 200);
    onBoardLED.setPixelColor(0, 0x20, 0x40, 0);
    onBoardLED.setPixelColor(1, 0x20, 0x40, 0);
    onBoardLED.show();
    delay(200);
    onBoardLED.clear();
    onBoardLED.show();
    switch (traffic_label) {
      case MU_TRAFFIC_CARD_TURN_AROUND:
        TankBase.writeAngle(30, 180);
        return;
      case MU_TRAFFIC_CARD_PARK:
        TankBase.write(0, 0);
        return;
      default:
        TankBase.write(0, 0);
        break;
    }
    unsigned long start_time = millis();
    int num = 0;
    static const int kPausePeriod = 600;
    while (millis()-start_time < kPausePeriod) {
      if (Mu_.GetValue(VISION_NUM_CARD_DETECT, kStatus)) {
        tone(tone_pin_, 1000, 400);
        onBoardLED.setPixelColor(0, 0x20, 0x40, 0x10);
        onBoardLED.setPixelColor(1, 0x20, 0x40, 0x10);
        onBoardLED.show();
        delay(400);
        onBoardLED.clear();
        onBoardLED.show();
        num = Mu_.GetValue(VISION_NUM_CARD_DETECT, kLabel);
        break;
      }
    }
    if (num > 0) {
      switch (traffic_label) {
        case MU_TRAFFIC_CARD_FORWARD:
          TankBase.writeDistance(30, num*10);
          while(TankBase.read(kLeftMotor)&&TankBase.read(kRightMotor));
          break;
        case MU_TRAFFIC_CARD_LEFT:
          TankBase.writeAngle(-30, num*90);
          while(TankBase.read(kLeftMotor)&&TankBase.read(kRightMotor));
          break;
        case MU_TRAFFIC_CARD_RIGHT:
          TankBase.writeAngle(30, num*90);
          while(TankBase.read(kLeftMotor)&&TankBase.read(kRightMotor));
          break;
        default:
          return;
      }
    } else {
      switch (traffic_label) {
        case MU_TRAFFIC_CARD_FORWARD:
          TankBase.writeRpm(30, 30);
          break;
        case MU_TRAFFIC_CARD_LEFT:
          TankBase.writeRpm(-30, 30);
          break;
        case MU_TRAFFIC_CARD_RIGHT:
          TankBase.writeRpm(30, -30);
          break;
        default:
          return;
      }
      delay(500);
    }
  }
}

