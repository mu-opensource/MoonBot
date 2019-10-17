#include <MoonBot.h>

MoonBotRover rover(Mu);

void setup() {
  Serial.begin(9600);
  Serial3.begin(115200);
  // enable MU on Serial3(Port9)
  Mu.begin(&Serial3);
  // enable TankBase
  TankBase.begin();
  // TankBase RPM correction
  TankBase.rpmCorrection(114);
  // TankBase straight distance correction
  TankBase.distanceCorrection(112);
  // TankBase turning angle correction
  TankBase.wheelSpacingSet(146);
  // rover initialized
  rover.followBallBegin();
}

void loop() {
  // Rover run.
  rover.runFollowBall();
}
