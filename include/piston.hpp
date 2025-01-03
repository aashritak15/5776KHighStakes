#ifndef PISTON_HPP
#define PISTON_HPP

#include "main.h"

inline pros::adi::DigitalOut mogoClamp('A');

void updateClamp();
void clampInit();

extern int clampState;

#endif