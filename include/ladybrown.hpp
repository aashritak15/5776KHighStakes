#ifndef LADYBROWN_HPP
#define LADYBROWN_HPP

#include "main.h"

inline pros::Motor ladyBrown(-5, pros::MotorGearset::green);

inline pros::Rotation lbRotation(20);

void ladyBrownInit();

void updateLB();
void runLB();

void updateLBTask();
void lbTask();

extern double globalTarget;

#endif