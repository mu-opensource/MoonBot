#include <MoonBot.h>

MoonBotMECH mech(Mu, m_servo[kServo1], m_servo[kServo3], m_servo[kServo4]);
MoonBotServo& claw = m_servo[kServo1];
MoonBotServo& lower_arm = m_servo[kServo3];
MoonBotServo& upper_arm = m_servo[kServo4];
int card_type_ = VISION_SHAPE_CARD_DETECT;       // set shoot ball target
enum mech_basketball_t {
  kSearchBall,
  kGrabBall,
  kSearchCard,
  kShootBall
};
mech_basketball_t mech_basketball_state = kSearchBall;

void setup() {
  Serial.begin(9600);
  Serial3.begin(115200);
  claw.attach(kServo1);               // link MECH claw to servo1
  lower_arm.attach(kServo3, true);    // link MECH lower arm to servo3, reverse direction
  upper_arm.attach(kServo4, true);    // link MECH upper arm to servo4, reverse direction
  TankBase.begin(false, false);       // disable encoder, and begin TankBase component
  Mu.begin(&Serial3);                 // enable MU vision sensor
  mech.card_center_x_ = 42;           // set follow card center X
  mech.shoot_card_width_ = 48;           // set follow card center X
  mech.ball_center_x_ = 50;           // set follow boll center X
  mech.begin();                       // enable MECH
}

void loop() {
  switch (mech_basketball_state) {
    case kSearchBall:
      // step 1: search ball
      if (mech.searchBall()) {
        // if MECH fin ball, go to step 2: grab ball
        mech_basketball_state = kGrabBall;
      }
      break;
    case kGrabBall:
      // step 2: grab ball
      switch (mech.grabBall()) {
        case kGrabedBall:
          // if MECH grab the ball, go to step 3: search card
          mech_basketball_state = kSearchCard;
          break;
        case kUndetectBall:
          // if MECH lost the ball， go to step 1: search ball
          mech_basketball_state = kSearchBall;
          break;
        default:
          break;
      }
      break;
    case kSearchCard:
      // step 3: search card
      if(mech.searchCard()) {
        // if MECH find card, go to step 4: shoot ball
        mech_basketball_state = kShootBall;
      }
      break;
    case kShootBall:
      // step 4: shoot ball
      switch(mech.shootBall()) {
        case kShootedBall:
          // if MECH shooted the ball, go to step 1: search ball
          mech_basketball_state = kSearchBall;
          break;
        case kUndetectCard:
          // if MECH lost card, go to step 3: search card
          mech_basketball_state = kSearchCard;
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

