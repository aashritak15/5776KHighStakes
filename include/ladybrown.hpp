#ifndef LADYBROWN_HPP
#define LADYBROWN_HPP


#include "main.h"

inline pros::Motor lift(16, pros::MotorGearset::red);
extern pros::Rotation rotationSensor;

void liftInit();
void rotationInit();
void updateLift();
void allianceStake();

#endif