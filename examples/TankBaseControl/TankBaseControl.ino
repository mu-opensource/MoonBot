#include <MoonBot.h>

void setup() {
  TankBase.rpmCorrection(100);                  // motor RPM(Revolutions Per Minute) correction(for function `writeRpm()`)
  TankBase.distanceCorrection(100);             // wheel distance correction(for function `writeDistance()`)
  TankBase.wheelSpacingSet(100);                // wheel spacing set and correction(for function `writeAngle()`)
  TankBase.begin();                             // enable wheel, reverse left motor directions and enable encoder component
}

void loop() {
  TankBase.writeRpm(30, 30);                    // set left and right motor speed 30RPM
  delay(5000);                                  // wait 5000ms for the wheel to forward
  TankBase.write(0, 0);                         // set the voltage of left and right motors to 0 to stop rotation
  delay(5000);                                  // stop 5000ms
  TankBase.writeDistance(30, 100);              // wheel forward 100cm at 30RPM
  while(TankBase.read(kLeftMotor) | TankBase.read(kRightMotor));      // waiting for wheel to stop
  TankBase.writeAngle(30, 360);                 // wheel rotate 360 clockwise
  while(TankBase.read(kLeftMotor) | TankBase.read(kRightMotor));      // waiting for wheel to stop
  delay(5000);                                  // stop 5000ms
}
