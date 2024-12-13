#include "main.h"

inline pros::Motor lift(16, pros::MotorGearset::red);
inline pros::Rotation rotationSensor(17);

void liftInit();
void rotationInit();
void updateLift();
void allianceStake();