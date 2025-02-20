#ifndef PISTON_HPP
#define PISTON_HPP

#include "main.h"
#include "globals.hpp"

inline pros::adi::DigitalOut mogoClamp('A');
inline pros::adi::DigitalOut doink('B');


void updateClamp();
void clampInit();
void updateDoink();
void doinkInit();
void runPistons();

extern int clampState;
extern int doinkState;

#endif