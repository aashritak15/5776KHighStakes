#ifndef INTAKE_HPP
#define INTAKE_HPP

#include "main.h"

inline pros::Motor intake1(14, pros::MotorGearset::blue); //changed from blue to green
inline pros::Motor intake2(17, pros::MotorGearset::green);
inline pros::Optical optical(16);

void intakeInit();

void updateIntake();
void updateColorSort();

void colorSort();
void antiJam();

extern int intakeState;
extern int sortState;
extern bool buttonUpPressed;
extern bool colorDetected;

#endif