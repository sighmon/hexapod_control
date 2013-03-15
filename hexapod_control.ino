//From the bildr article: http://bildr.org/2012/03/servos-tlc5940-arduino
//Requires the Tlc5940 library. http://code.google.com/p/tlc5940arduino/downloads/list

// Had to edit tlc_config.h #defin NUM_TLCS

#include "Tlc5940.h"
#include "tlc_servos.h"

int numberOfServos = 18; //how many servos on the chain?

#define FRONT 0
#define MIDDLE 1
#define REAR 2
#define INNER 0 // AKA centre
#define OUTER 2 // AKA top 

#define LEFT 0
#define RIGHT 1

void setup(){
  tlc_initServos();  // Note: this will drop the PWM freqency down to 50Hz.
}

// Leg segment position

void setLegSegmentPosition(int pair, int hand, int segment, int angle) {
  int servoPos = angle;
  int servo;
  switch (pair) {
    case FRONT:
      servo = 0;
      break;
    case MIDDLE:
      servo = 6;
      break;
    case REAR:
      servo = 12;
      break;
  }
  switch (hand) {
    case LEFT:
      // do nothing
      break;
    case RIGHT:
      servo += 3;
      break;
  }
  switch (segment) {
    case OUTER:
      // do nothing
      break;
    case MIDDLE:
      servo += 1;
      break;
    case INNER:
      servo += 2;
      break;
  }
  if ((servo == 0) || (servo == 6) || (servo == 7) || (servo == 10) || (servo == 15) || (servo == 16)) {
    servoPos = (180 - angle);
  }
  if (segment == MIDDLE) {
    servoPos = (180 - servoPos);
  }
  if ((pair != MIDDLE) && (hand == RIGHT) && (segment == INNER)) {
    servoPos = (180 - servoPos);
  }
  tlc_setServo(servo, servoPos);
}

// Set full leg

void setLegPosition(int pair, int hand, int inner, int middle, int outer) {
   setLegSegmentPosition(pair, hand, INNER, inner);
   setLegSegmentPosition(pair, hand, MIDDLE, middle);
   setLegSegmentPosition(pair, hand, OUTER, outer);
}

// Neutral stance

void neutralStance() {
  for (int pair = 0; pair < 3; pair++) {
    for (int hand = 0; hand < 2; hand++) {
      for (int segment = 0; segment < 3; segment++) {
        setLegSegmentPosition(pair, hand, segment, 90);
      }
    } 
  }
  // Tlc.update();
}

// Outer clench

void outerClench() {
  for (int pair = 0; pair < 3; pair++) {
    for (int hand = 0; hand < 2; hand++) {
      for (int segment = 0; segment < 3; segment++) {
        setLegSegmentPosition(pair, hand, segment, (segment==OUTER ? 0 : 90));
      }
    } 
  }
  Tlc.update();
}

// Outer stretch

void outerStretch() {
  for (int pair = 0; pair < 3; pair++) {
    for (int hand = 0; hand < 2; hand++) {
      for (int segment = 0; segment < 3; segment++) {
        setLegSegmentPosition(pair, hand, segment, (segment==OUTER ? 180 : 90));
      }
    } 
  }
  Tlc.update();
}

// Outer stretch animation

void outerStretchAnimation() {
  for (int pair = 0; pair < 3; pair++) {
    for (int hand = 0; hand < 2; hand++) {
      neutralStance();
      for (int segment = 0; segment < 3; segment++) {
        setLegSegmentPosition(pair, hand, segment, (segment==OUTER ? 180 : 90));
      }
      Tlc.update();
      delay(1000);
    } 
  }
}

// Middle & outer stretch animation

void middleAndOuterStretchAnimation() {
  for (int pair = 0; pair < 3; pair++) {
    for (int hand = 0; hand < 2; hand++) {
      neutralStance();
      for (int segment = 0; segment < 3; segment++) {
        setLegSegmentPosition(pair, hand, segment, (((segment==MIDDLE) || (segment==OUTER)) ? 180 : 90));
      }
      Tlc.update();
      delay(1000);
    } 
  }
}

// Inner forward animation

void innerForwardAnimation() {
  for (int pair = 0; pair < 3; pair++) {
    for (int hand = 0; hand < 2; hand++) {
      neutralStance();
      for (int segment = 0; segment < 3; segment++) {
        setLegSegmentPosition(pair, hand, segment, (segment==INNER ? 120 : 90));
      }
      Tlc.update();
      delay(1000);
      neutralStance();
      Tlc.update();
      delay(1000);
    } 
  }
}

// Walk pose

void walkPose(int set, int rotation, int height) {
  int neutral = 90;
  int middle = neutral + height;
  int outer = neutral - height;
  if (set == 0) {
    setLegPosition(FRONT, LEFT, rotation, middle, outer);
    setLegPosition(REAR, LEFT, rotation, middle, outer);
    setLegPosition(MIDDLE, RIGHT, (180 - rotation), middle, outer);
  } else if (set == 1) {
    setLegPosition(FRONT, RIGHT, (180 - rotation), middle, outer);
    setLegPosition(REAR, RIGHT, (180 - rotation), middle, outer);
    setLegPosition(MIDDLE, LEFT, rotation, middle, outer);
  }
}

// Walk animation

// TODO: FIX (add) ANIMATION STEP

void walkAnimation() {
  walkPose(1, 120, 0);
  walkPose(0, 60, 90);
  Tlc.update();
  delay(1000);
  walkPose(0, 120, 90);
  Tlc.update();
  delay(1000);
  walkPose(0, 120, 0);
  Tlc.update();
  delay(1000);
  walkPose(1, 120, 90);
  Tlc.update();
  delay(1000);
  walkPose(1, 60, 90);
  Tlc.update();
  delay(1000);
  walkPose(1, 60, 0);
  Tlc.update();
  delay(1000);
  walkPose(0, 60, 0);
  Tlc.update();
  delay(1000);
  walkPose(0, 60, 90);
  Tlc.update();
  delay(1000);
  walkPose(0, 120, 90);
  Tlc.update();
  delay(1000);
  walkPose(1, 60, 90);
  Tlc.update();
  delay(1000);
  walkPose(1, 120, 90);
  Tlc.update();
  delay(1000);
  walkPose(1, 120, 0);
  Tlc.update();
  delay(1000);
}

void loop(){
  neutralStance();
  //Tlc.update();
  // delay(1000);
  //outerClench();
  //delay(1000);
  //neutralStance();
  //delay(1000);
  //outerStretch();
  //middleAndOuterStretchAnimation();
  // innerForwardAnimation();
  walkAnimation();
  //delay(1000);
}
