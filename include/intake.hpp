#ifndef INTAKE_HPP
#define INTAKE_HPP

#include "main.h"

inline pros::Motor intake(14, pros::MotorGearset::blue); //changed from blue to green
inline pros::Optical optical(8);

void intakeInit();
void opticalInit();

void updateIntake();
void updateColorSort();

void colorSort(int lol);

extern int intakeState;
extern int sortState;
extern bool buttonUpPressed;
extern bool colorDetected;

#endif