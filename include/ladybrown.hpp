#ifndef LADYBROWN_HPP
#define LADYBROWN_HPP


#include "main.h"

inline pros::Motor lift(-17, pros::MotorGearset::red);
extern pros::Rotation rotationSensor;

void liftInit();
void updateLB();
void rotationInit();
void updateLift();
void allianceStake();
void updateLB();


void updateLadyTask(double target);

#endif