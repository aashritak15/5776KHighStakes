#ifndef LADYBROWN_HPP
#define LADYBROWN_HPP


#include "main.h"

inline pros::Motor ladyBrown(-17, pros::MotorGearset::red);
// inline pros::Rotation rotationSensor(13);
inline pros::Rotation lbRotation(13); //AKANKSH CHECK IF THE PORT IS RIGHT

void ladyBrownInit();
void updateLB();
void rotationInit();
void updateLift();
void allianceStake();
void updateLB();

//extern double ladyTarget;

void updateLadyTask(double target);
void autonLB(double ladyTarget, int timeout);

#endif