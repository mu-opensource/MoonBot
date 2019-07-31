#include <MoonBot.h>

int32_t m_accelerometer[3] = { 0 };

void setup() {
  IMU.enable();                                                       // enable IMU on MoonBot controller
  IMU.calibrateMag();                                                 // IMU calibration, you need to turn the IMU in a '∞' shape
  Serial.begin(9600);
  LED.begin();
}

void loop() {
  Serial.println("========================================");
  Serial.print("compass:");
  Serial.println(IMU.getMagAngle());                                  // get compass angle, value from 0 to 360 degrees, if the Y axes pointing to the north, the IMU will return a value of 0 or 360
  Serial.print("pitch:");
  Serial.print(IMU.getAccAngle(kAccPitch));                           // get pitch angle, value from -180 to 180 degrees, if the Y axis direction is up, return value is positive, otherwise negative
  Serial.print("  roll:");
  Serial.println(IMU.getAccAngle(kAccRoll));                          // get roll angle, value from -180 to 180 degrees, if the controller tilts to the right, return value is positive, otherwise negative
  Serial.print("temperature:");
  Serial.println(IMU.temperatureC());                                 // get temperature value(℃)
  IMU.Acc.GetAxes(m_accelerometer);                                   // get accelerometer X/Y/Z axes value
  Serial.print("accelerometer: x=");                                  // print value
  Serial.print(m_accelerometer[0]);
  Serial.print(" y=");
  Serial.print(m_accelerometer[1]);
  Serial.print(" z=");
  Serial.println(m_accelerometer[2]);
  Serial.print("Gravity acceleration=");
  Serial.println(IMU.getAcceleration());                              // get gravity acceleration
  if (IMU.on(kIMUShake)) {                                            // if the IMU is on shake, LED show red
    LED.setPixelColor(0, 0xff0000);
    LED.setPixelColor(1, 0xff0000);
    LED.show();
  } else if (IMU.on(kIMUFreeFall)) {                                  // if the IMU is falling freely, LED show green
    LED.setPixelColor(0, 0x00ff00);
    LED.setPixelColor(1, 0x00ff00);
    LED.show();
  } else {
    LED.setPixelColor(0, 0x000000);
    LED.setPixelColor(1, 0x000000);
    LED.show();
  }
}
