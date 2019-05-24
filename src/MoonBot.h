/*
 * MoonBot.h
 *
 *  Created on: 2019年3月14日
 *      Author: ysq
 */

#ifndef MOONBOT_MOONBOT_H_
#define MOONBOT_MOONBOT_H_

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <SoftwareWire.h>
#include <Adafruit_NeoPixel.h>
#include <MoonBot_TankBase.h>
#include <MoonBot_WT2003S_MP3_Decoder.h>
#include <MuVisionSensor.h>

#include "pins_moonbot.h"
#include "LSM303AGR_IMU_Sensor.h"
#include "MoonBot_Servo.h"
#include "MoonBot_Eyes.h"
#include "MoonBot_MECH.h"
#include "MoonBot_Rover.h"

extern MuVisionSensor Mu;
extern Adafruit_NeoPixel onBoardLED;
extern WT2003S speaker;

#define LED                                 onBoardLED

#ifdef __AVR_ATmega1280__
void moonbotInit(int start_button = MOONBOT_PIN_BUTTON_A);
bool isButtonPressed(int key_num = MOONBOT_PIN_BUTTON_A);
#endif

#endif /* MOONBOT_MOONBOT_H_ */
