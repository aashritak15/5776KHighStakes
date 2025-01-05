#ifndef MAGIC_HPP
#define MAGIC_HPP

#include "main.h"
#include "globals.hpp"
#include "intake.hpp"
#include "piston.hpp"
#include <iostream>
#include <fstream>
#include <string>

inline std::ofstream fileO;
inline std::ofstream fileOTwo;

extern bool active;

extern int prevIntakeState;
extern int prevClampState;

void initO();
void initDebug();

void closeO();

void writePose();
void writeAdditional();

//jerry

// extern std::vector<Waypoint> route; 

// void followRoute(const std::vector<Waypoint>& route);
// void readRouteFile; 

#endif
