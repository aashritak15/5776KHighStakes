#include "main.h"

inline pros::Motor lift(16, pros::MotorGearset::red);
inline pros::Rotation rotationSensor(18);

void liftInit();
void updateLift();
void allianceStake();