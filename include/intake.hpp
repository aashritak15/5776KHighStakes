#ifndef INTAKE_HPP
#define INTAKE_HPP

#include "main.h"

inline pros::Motor intakeUpper(14, pros::MotorGearset::blue);
inline pros::Motor intakeLower(-1, pros::MotorGearset::green); //TODO: find port
inline pros::Optical optical(16);

void intakeInit();

void updateIntake();
void updateColorSort();

void runColorSort();
void runIntake();
void antiJam();

extern int intakeState;
extern int sortState;
extern bool buttonUpPressed;
extern bool colorDetected;

#endif