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
inline std::ofstream fileOThree;

// inline std::ofstream newPoseFile;
// inline std::ofstream newExtraFile;

extern bool active;

extern int prevIntakeState;
extern int prevClampState;
extern float prevError;

void initO();
void initDebug();

void closeO();

void writePose();
void writeAdditional();

void addSegment();

void rerunPIDs();

//jerry

// extern std::vector<Waypoint> route; 

// void followRoute(const std::vector<Waypoint>& route);
// void readRouteFile; 

#endif
