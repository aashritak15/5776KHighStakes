#include "main.h"

extern pros::Controller controller;

extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;

extern pros::Imu imu;

// extern pros::Rotation vertical;
// extern pros::Rotation horizontal;

extern lemlib::TrackingWheel vertical;
extern lemlib::TrackingWheel horizontal;

extern lemlib::Drivetrain drivetrain;
extern lemlib::ControllerSettings linearController;
extern lemlib::ControllerSettings angularController;

extern lemlib::OdomSensors sensors;

extern lemlib::ExpoDriveCurve throttleCurve;
extern lemlib::ExpoDriveCurve steerCurve;

extern lemlib::Chassis chassis;

void mainLoop();
void interruptLoop();