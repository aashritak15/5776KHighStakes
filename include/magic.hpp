#ifndef MAGIC_HPP
#define MAGIC_HPP

#include "globals.hpp"
#include <fstream>
#include <string>

extern int section;

inline std::ofstream fileO;
inline std::ofstream fileOTwo;
inline std::ofstream fileOThree;

inline std::ifstream fileI;
inline std::ifstream fileITwo;

inline std::ofstream fileInterrupt;
inline std::ofstream fileInterruptTwo;

inline std::ofstream reflector;
inline std::ofstream reflectorTwo;

// inline std::ofstream newPoseFile;
// inline std::ofstream newExtraFile;

extern bool buttonPressed;

extern int prevIntakeState;
extern int prevClampState;
extern float prevError;

void initO();
void initDebug();
void initInterrupt(int lastSection, int stopIndex);
// void initButtonInterrupt(int stopIndex);

void closeO();
void closeOInterrupt();

void writePose();
void writeAdditional();

void writeInterruptPose();
void writeInterruptAdditional();

void addSegment();

void rerunPIDs();

void reflect(bool x, bool y);

std::vector<std::string> readElementMagic(const std::string& input, const std::string& delimiter);

//jerry

// extern std::vector<Waypoint> route; 

// void followRoute(const std::vector<Waypoint>& route);
// void readRouteFile; 

#endif
