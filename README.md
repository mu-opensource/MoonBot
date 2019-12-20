[![Morpx-bbs](http://bbs.morpx.com/template/mu/images/logo.png)](http://bbs.morpx.com/forum.php)
[![Git-rep](https://img.shields.io/github/repo-size/mu-opensource/MoonBot.svg)](https://github.com/mu-opensource/MoonBot)
[![Git-release](https://img.shields.io/github/downloads/mu-opensource/MoonBot/total.svg)](https://github.com/mu-opensource/MoonBot/releases)


# MoonBot

[![Pic-MoonBot](http://mai.morpx.com/images/moonbot/moonbot.png)](http://mai.morpx.com/)

MoonBot is a robot kit designed and developed by Morpx Inc.

The goal of MoonBot is to inspire kids to connect what they learned in school to projects that have real world impacts. MoonBot is equipped with state-of-the-art AI sensing technology, making it one of the smartest robots on the market.

This repository will share all the open-source code running on MoonBot, as well as its instruction manuals, example programs, etc.

You can use these libraries to read data or set properties of the MU Vision Sensor.

Major components in this library:

* MoonBot.h -- include all driver in MoonBot
* pins_moonbot.h -- pin definition functions for MoonBot
* MoonBot_Motor.h -- MoonBot single motor driver
* MoonBot_TankBase.h -- MoonBot tank base driver
* MoonBot_Servo.h -- MoonBot servo driver
* MoonBot_WT2003S_MP3_Decoder.h -- MoonBot MP3 player driver
* LSM303AGR_IMU_Sensor.h -- MoonBot IMU driver
* MoonBot_MECH.h -- MoonBot MECH driver
* MoonBot_Rover.h -- MoonBot rover driver
* MoonBot_Bot.h --  MoonBot robot driver

## Usage

### Supported chipsets

We have included code for **Arduino ATmega 1280** only, **DO NOT** use moonbot library on other Arduino chips. 

### Before you use

To program **MoonBot** in Arduino IDE, you need to install the following libraries first:
[`How to Installing Additional Arduino Libraries?`](https://www.arduino.cc/en/Guide/Libraries/?setlang=en)

Download .zip libraries and install (Sketch -> Include Library -> Add .ZIP Library ...)
* MoonBot (on this page, click the green "Clone or download" button)
* [MuVisionSensorIII](https://github.com/mu-opensource/MuVisionSensorIII)

Install via Arduino Library Manager (Tools -> Manage Libraries... then search for the library name)
* Servo
* Adafruit_NeoPixel
* AsyncDelay
* SoftwareWire
* MsTimer2

MoonBot also uses these libraries that are included with default Arduino installation
* SoftwareSerial
* Wire

### Simple Example

#### Servo Driver

```cpp
/* Sweep servo */
#include <MoonBot.h>
MoonBotServo myservo;
int pos;
void setup() {
    m_servo[kServo1].attach(kServo1, true); // attaches servo on servo port 1, and reverse directions
}
void loop() {
  for (pos = 0; pos <= 180; pos += 1) {     // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    m_servo[kServo1].write(pos);            // tell servo to go to position in variable 'pos'
    delay(15);                              // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) {     // goes from 180 degrees to 0 degrees
    m_servo[kServo1].write(pos);            // tell servo to go to position in variable 'pos'
    delay(15);                              // waits 15ms for the servo to reach the position
  }
}
```

#### TankBase Driver

```cpp
#include <MoonBot.h>
void setup() {
    TankBase.rpmCorrection(100);               // motor RPM(Revolutions Per Minute) correction(for function `writeRpm()`)
    TankBase.distanceCorrection(100);          // wheel distance correction(for function `writeDistance()`)
    TankBase.wheelSpacingSet(100);             // wheel spacing set and correction(for function `writeAngle()`)
    TankBase.begin(true, false, true);         // enable wheel, reverse left motor directions and enable encoder component
}
void loop() {
  TankBase.writeRpm(30, 30);                   // set left and right motor speed 30RPM
  delay(5000);                                 // wait 5000ms for the wheel to forward
  TankBase.write(0, 0);                        // set the voltage of left and right motors to 0 to stop rotation
  delay(5000);                                 // stop 5000ms
  TankBase.writeDistance(30, 100);             // wheel forward 100cm at 30RPM
  while(TankBase.read(kLeftMotor) | TankBase.read(kRightMotor));      // waiting for wheel to stop
  TankBase.writeAngle(30, 360);                // wheel rotate 360 clockwise
  while(TankBase.read(kLeftMotor) | TankBase.read(kRightMotor));      // waiting for wheel to stop
  delay(5000);                                 // stop 5000ms
}
```

#### MP3 Player Driver

```cpp
#include <MoonBot.h>
void setup() {
    char music_num[] = "0101";
    speaker.begin(Serial1);                 // enable speaker on port 7(hardware serial 1)
    speaker.setVolume(32);                  // set speaker volume to 32/32(100%)
    speaker.playFileName(music_num);        // play music "0101xxxx.mp3"
}
void loop() {
}
```

#### LED Driver

```cpp
#include <MoonBot.h>
void ledSetColor(Adafruit_NeoPixel& led, uint32_t c) {
    led.fill(c, 0, 0);
    led.show();
}
void setup() {
    LED.begin();                            // enable controller LED
    moonbot_eyes.begin();                   // enable eyes LED
}
void loop() {
    ledSetColor(LED, 0xFF00FF);             // set controller LED color to purple
    ledSetColor(moonbot_eyes, 0x00FF00);    // set eyes to green
    delay(500);                             // delay 500ms
    ledSetColor(moonbot_eyes, 0xFF00FF);    // set eyes to purple
    ledSetColor(LED, 0x00FF00);             // set controller LED color to green
    delay(500);                             // delay 500ms
}
```

#### IMU Driver

```cpp
#include <MoonBot.h>
void setup() {
    LED.begin();                                // enable controller LED
    IMU.enable();                               // enable IMU
    IMU.calibrateMag();                         // IMU magnetometer correction
}
void loop() {
    int angle = IMU.getMagAngle();              // get angle with geomagnetic
    Serial.print("angle_y_x = ");               // print angle
    Serial.println(angle);
    int roll = IMU.getAccAngle(kAccRoll);       // get acceleration roll angle
    Serial.print("roll = ");                    // print angle
    Serial.println(roll);
    int pitch = IMU.getAccAngle(kAccPitch);     // get acceleration pitch angle
    Serial.print("pitch = ");                   // print angle
    Serial.println(pitch);

    if (IMU.on(kIMUShake)) {
        // if IMU on shake
        LED.fill(0xFF0000, 0, 0);               // LED show red
        LED.show();
    } else if (IMU.on(kIMUFreeFall)) {
        // if IMU on free fall
        LED.fill(0x00FF00, 0, 0);               // LED show green
        LED.show();   
    } else {
        // close controller LED
        LED.clear();
        LED.show();
    }
}
```

#### MU Vision Sensor Driver
Visit [https://github.com/mu-opensource/MuVisionSensorIII](https://github.com/mu-opensource/MuVisionSensorIII) for details 


## For more information

Check out the official site [http://www.morpx.com/zn.index.html](http://www.morpx.com/zn.index.html) for links to documentation, issues, and news


