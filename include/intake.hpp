#include "main.h"

inline pros::Motor intake(9, pros::MotorGearset::green); //changed from blue to green

void intakeInnit();
void updateIntake();
void resetIntake();
void stepIntake();

extern int intakeState;