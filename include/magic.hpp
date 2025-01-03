#ifndef MAGIC_HPP
#define MAGIC_HPP

#include "main.h"
#include "globals.hpp"
#include "intake.hpp"
#include <iostream>
#include <fstream>
#include <string>

inline std::ofstream fileO;
inline std::ofstream fileOTwo;
inline std::ofstream fileOThree;

inline std::ifstream fileI;
inline std::ifstream fileITwo;

// inline std::ofstream fileOExtra; TODO: we don't use these anywhere right
// inline std::ifstream fileIExtra;

extern bool active;
extern int count;

void initO();
void initI();
void initDebug();

void closeO();
void closeI();

void writePose();
void writeAdditional();

//jerry

// extern std::vector<Waypoint> route; 

// void followRoute(const std::vector<Waypoint>& route);
// void readRouteFile; 

#endif
