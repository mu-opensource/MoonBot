/*
 * MoonBot.h
 *
 *  Created on: 2019年3月14日
 *      Author: ysq
 */

#ifndef MOONBOT_MOONBOT_H_
#define MOONBOT_MOONBOT_H_

#ifndef __AVR_ATmega1280__
#error "Please use arduino board: ATmega1280"
#endif

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
#include "MoonBot_BuzzerPlayer.h"
#include "MoonBot_Eyes.h"
#include "MoonBot_MECH.h"
#include "MoonBot_Rover.h"
#include "MoonBot_Humannoid.h"

// The following two diagrams show how the sensors, servos and motors should
// be connected to the main controller of the MoonBot.
//
//                              USB          Battery
//                   ┌───────┐ TypeC ┌──────┐ ||||| ┌───────┐
//                   │       └───────┘      └───────┘       │
//                   └─┐                                  ┌─┘
//                     │                                  │
//          EyeLED - P1│             MoonBot              │P9 - MuVisionSensor
//                   ┌─┘         Main Controller          └─┐ (Arduino Serial3)
//                   └─┐          Bottom Layer            ┌─┘
//                     │                                  │
//         Speaker - P2│    (BTN A)           (BTN B)     │P8
//(Arduino Serial2)  ┌─┘                                  └─┐
//                   └─┐                                  ┌─┘
//     Right IRSensor/ │                                  │     Left IRSensor/
//     TouchSensor - P3│             (RESET)              │P7 -  TouchSensor
//                   ┌─┘                                  └─┐  (Arduino Serial1)
//                   │     ┌──────┐  ┌──────┐  ┌──────┐     │
//                   └─────┘  P4  └──┘  P5  └──┘  P6  └─────┘
//                        Right Motor  (I2C)   Left Motor
//                          Encoder             Encoder
//
//
//                              USB          Battery
//                   ┌───────┐ TypeC ┌──────┐ ||||| ┌───────┐
//                   │       └───────┘      └───────┘       │
//        Servo1     └─┐                                  ┌─┘   Servo4
//     Mech: Claw      │                                  │  Mech: Upper Arm
//     Bot:  Right Arm │              MoonBot             │  Bot:  Left Arm
//                   ┌─┘          Main Controller         └─┐
//                   │              Upper Layer             │
//                   │                                      │
//                   │      (BTN A)            (BTN B)      │
//                   │                                      │
//                   └─┐                                  ┌─┘   Servo3
//                     │                                  │  Bot/Rover: Head
//          Servo2     │             (RESET)              │  Mech:      Lower Arm
//                   ┌─┘                                  └─┐
//                   │       ┌────┐            ┌────┐       │
//                   └───────┘ M1 └────────────┘ M2 └───────┘
//                       Right Motor       Left Motor

/*
 * @brief: You can use these libraries to read data or set properties of the MU Vision Sensor.
 *         see `https://github.com/mu-opensource/MuVisionSensorIII/` for detail
 * @simple example:
 *  #include <MoonBot.h>
 *  #define VISION_TYPE VISION_BALL_DETECT
 *  void setup() {
 *    Wire.begin();
 *    Mu.begin(&Wire, kI2CMode);
 *    Mu.VisionBegin(VISION_TYPE);
 *  }
 *  void loop() {
 *    if (Mu.GetValue(VISION_TYPE, kStatus)) {
 *      int x = Mu.GetValue(VISION_TYPE, kXValue);
 *      int y = Mu.GetValue(VISION_TYPE, kYValue);
 *      int width = Mu.GetValue(VISION_TYPE, kWidthValue);
 *      int height = Mu.GetValue(VISION_TYPE, kHeightValue);
 *      int label = Mu.GetValue(VISION_TYPE, kLabel);
 *    }
 *  }
 */
extern MuVisionSensor Mu;
/*
 * @brief: LED for moonbot eyes and on board LED
 * @simple example:
 *  #include <MoonBot.h>
 *  void ledSetColor(Adafruit_NeoPixel& led, uint32_t c) {
 *    for (uint16_t i = 0; i < led.numPixels(); ++i) {
 *        led.setPixelColor(i, c);
 *    }
 *    led.show();
 *  }
 *  void setup() {
 *    onBoardLED.begin();
 *    moonbot_eyes.setPin(moonbotPortToPin(kPort1, kPortPin1));   // initialized moonbot eyes on port1
 *    moonbot_eyes.begin();
 *  }
 *  void loop() {
 *    ledSetColor(onBoardLED, 0xFF00FF);      // set on board led color to purple
 *    ledSetColor(moonbot_eyes, 0x00FF00);    // set eyes to green
 *    delay(500);                             // delay 500ms
 *    ledSetColor(moonbot_eyes, 0xFF00FF);    // set eyes to purple
 *    ledSetColor(onBoardLED, 0x00FF00);      // set on board led color to green
 *    delay(500);
 *    }
 *  }
 */
extern Adafruit_NeoPixel onBoardLED;
extern Adafruit_NeoPixel moonbot_eyes;
/*
 * @brief: MoonBot speaker
 * @simple example:
 * #include <MoonBot.h>
 *  void setup() {
 *    char music_num[] = "0101";
 *    speaker.begin(Serial1);                 // enable speaker on port 7(hardware serial 1)
 *    speaker.setVolume(32);                  // set speaker volume to 32/32(100%)
 *    speaker.playFileName(music_num);        // play music "0101xxxx.mp3"
 *  }
 */
extern WT2003S speaker;
/*
 * @brief: MoonBot IMU
 * @simple example:
 * #include <MoonBot.h>
 *  void setup() {
 *    IMU.enable();                               // enable IMU
 *    IMU.calibrateMag();                         // IMU magnetometer correction
 *  }
 *  void loop() {
 *    int angle = IMU.getMagAngle();              // get angle with geomagnetic
 *    Serial.print("angle_y_x = ");               // print angle
 *    Serial.println(angle);
 *    int roll = IMU.getAccAngle(kAccRoll);       // get acceleration roll angle
 *    Serial.print("roll = ");                    // print angle
 *    Serial.println(roll);
 *    int pitch = IMU.getAccAngle(kAccPitch);     // get acceleration pitch angle
 *    Serial.print("pitch = ");                   // print angle
 *    Serial.println(pitch);
 *    if (IMU.on(kIMUFreeFall)) {                 // is IMU on free fall?
 *        Serial.println("IMU is shaking!");
 *    }
 *    if (IMU.on(kIMUShake)) {                    // is IMU on shake?
 *        Serial.println("IMU is free fall!");
 *    }
 *  }
 */
extern LSM303AGR_IMU_Sensor IMU;
/*
 * @brief: MoonBot tank base
 * @simple example:
 *  #include <MoonBot.h>
 *  void setup() {
 *    TankBase.rpmCorrection(100);               // motor RPM(Revolutions Per Minute) correction(for function `writeRpm()`)
 *    TankBase.distanceCorrection(100);          // wheel distance correction(for function `writeDistance()`)
 *    TankBase.wheelSpacingSet(100);             // wheel spacing set and correction(for function `writeAngle()`)
 *    TankBase.begin(true, false, true);         // enable wheel, reverse left motor directions and enable encoder component
 *  }
 *  void loop() {
 *    TankBase.writeRpm(30, 30);                   // set left and right motor speed 30RPM
 *    delay(5000);                                 // wait 5000ms for the wheel to forward
 *    TankBase.write(0, 0);                        // set the voltage of left and right motors to 0 to stop rotation
 *    delay(5000);                                 // stop 5000ms
 *    TankBase.writeDistance(30, 100);             // wheel forward 100cm at 30RPM
 *    while(TankBase.read(kLeftMotor) | TankBase.read(kRightMotor));      // waiting for wheel to stop
 *    TankBase.writeAngle(30, 360);                   // wheel rotate 360 clockwise
 *    while(TankBase.read(kLeftMotor) | TankBase.read(kRightMotor));      // waiting for wheel to stop
 *    delay(5000);                                 // stop 5000ms
 *  }
 */
extern MoonBotTankBase TankBase;
/*
 * @brief: MoonBot servo
 * @simple example:
 *  #include <MoonBot.h>
 *  int pos;
 *  void setup() {
 *      m_servo[kServo1].attach(kServo1, true);          // attaches servo on servo port 1, and reverse directions
 *  }
 *  void loop() {
 *    for (pos = 0; pos <= 180; pos += 1) {     // goes from 0 degrees to 180 degrees
 *      // in steps of 1 degree
 *      m_servo[kServo1].write(pos);            // tell servo to go to position in variable 'pos'
 *      delay(15);                              // waits 15ms for the servo to reach the position
 *    }
 *    for (pos = 180; pos >= 0; pos -= 1) {     // goes from 180 degrees to 0 degrees
 *      m_servo[kServo1].write(pos);            // tell servo to go to position in variable 'pos'
 *      delay(15);                              // waits 15ms for the servo to reach the position
 *    }
 *  }
 */
extern MoonBotServo m_servo[kServoNum];

#define LED                                 onBoardLED

void moonbotInit(int start_button = MOONBOT_PIN_BUTTON_A);
bool isButtonPressed(int key_num = MOONBOT_PIN_BUTTON_A);

#endif /* MOONBOT_MOONBOT_H_ */
