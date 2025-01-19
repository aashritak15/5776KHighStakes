#ifndef INTAKE_HPP
#define INTAKE_HPP


#include "main.h"

inline pros::Motor intake(14, pros::MotorGearset::blue); //changed from blue to green
inline pros::Optical optical(8); //TODO: ADD THE PORT

void intakeInnit();
void updateIntake();
void resetIntake();
void stepIntake();
void opticalInit();

void colorSort(int lol);
void updateColorToggle();

extern int intakeState;
extern int sortState;

extern int intakeState;
extern int sortState;
extern int prevSortState;
extern int stateState;

// extern bool buttonUpPressed;
// extern bool colorDetected;

#endif