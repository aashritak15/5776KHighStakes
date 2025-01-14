#ifndef INTAKE_HPP
#define INTAKE_HPP


#include "main.h"

inline pros::Motor intake(14, pros::MotorGearset::blue); //changed from blue to green
inline pros::Optical optical(14);

void intakeInnit();
void updateIntake();
void resetIntake();
void stepIntake();
void opticalInit();

void colorSort();
void updateColorToggle();

extern int intakeState;
extern int sortState;

#endif