#ifndef PISTON_HPP
#define PISTON_HPP

#include "main.h"
#include "globals.hpp"

inline pros::adi::DigitalOut mogoClamp('A');
inline pros::adi::DigitalOut doinkRight('B');
inline pros::adi::DigitalOut doinkLeft('C');
inline pros::adi::DigitalOut intakePiston('D');

void updateClamp();
void clampInit();
void updateDoinkRight();
void updateDoinkLeft();
void updateIntakePiston();
void doinkInit();
void runPistons();

extern int clampState;
extern int doinkRightState;
extern int doinkLeftState;
extern int intakePistonState;

#endif