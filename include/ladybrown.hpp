#ifndef LADYBROWN_HPP
#define LADYBROWN_HPP

#include "main.h"

inline pros::Motor ladyBrown(-5, pros::MotorGearset::red);
// inline pros::Rotation rotationSensor(13);
inline pros::Rotation lbRotation(20);

void ladyBrownInit();

void updateLB();
void runLB();

// extern double ladyTarget;

// void updateLadyTask(double target);
// void autonLB(double ladyTarget, int timeout);
void lbTask();

extern double globalTarget;

#endif