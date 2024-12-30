#include "main.h"

inline pros::Motor lift(17, pros::MotorGearset::red);
inline pros::Rotation rotationSensor(17);

void liftInit();
void rotationInit();
void updateLift();
void allianceStake();