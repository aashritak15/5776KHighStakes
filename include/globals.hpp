#include "main.h"

extern pros::Controller controller;

extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;


extern pros::Rotation vertical;
extern pros::Rotation horizontal;

extern lemlib::TrackingWheel verticalTracker;
extern lemlib::TrackingWheel horizontalTracker;

extern lemlib::Drivetrain drivetrain;
extern lemlib::ControllerSettings linearController;
extern lemlib::ControllerSettings angularController;

extern lemlib::OdomSensors sensors;

extern lemlib::ExpoDriveCurve throttleCurve;
extern lemlib::ExpoDriveCurve steerCurve;

extern lemlib::Chassis chassis;