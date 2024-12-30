#include "main.h"

inline pros::adi::DigitalOut mogoClamp('A');
inline pros::adi::DigitalOut doinker('B');

void updateClamp();
void updateDoinker();
void clampInit();