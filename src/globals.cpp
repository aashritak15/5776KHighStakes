#include "globals.hpp"
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "ports.hpp"
#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "ladybrown.hpp"
#include "intakePiston.hpp"
#include "autons.hpp"
#include "magic.hpp"
#include <cmath>

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::MotorGroup leftMotors({-8, -5, -11}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({7, 21, 18}, pros::MotorGearset::blue);

// pros::Rotation vertical(-1);
// pros::Rotation horizontal(10); //TODO: check later

pros::Imu imu(11);

// pros::Rotation verticalEnc(1);

// lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_275, 0.125);

// pros::Rotation horizontalEnc(10);

// lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_275, -2.5);

// lemlib::TrackingWheel verticalTracker(&vertical, lemlib::Omniwheel::NEW_275, 0.125);
// lemlib::TrackingWheel horizontalTracker(&horizontal, lemlib::Omniwheel::NEW_275, -2.5);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, 12.5, lemlib::Omniwheel::NEW_275, 450,
                              8 // TODO:
);

// lateral motion controller
lemlib::ControllerSettings linearController(12, // proportional gain (kP) //TODO: tune pids!!!
                                            0, // integral gain (kI)
                                            4, // 48.905, //46.86273, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            2, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            0 // slew
);

// angular motion controller
lemlib::ControllerSettings angularController(1.5, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             7, // 38,//37.88, // 32.92, //40.5, // derivative gain (kD)
                                             0, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // slew

                                             // OLD VALUES OCT 6: P 4.05, D 34.86768
);
// sensors for odometry
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null //TODO: what the actual flip
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            nullptr // inertial sensor
);

// tanish driver functions:

lemlib::ExpoDriveCurve throttleCurve(5, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(5, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

void mainLoop() {
    chassis.calibrate();

    initO();

    int count = 1;

    while (true) {
      
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        chassis.arcade(leftY, rightX * 0.9);

        updateIntake();
        updateClamp();
        //updateLadyPID();
        //updateLadyTask();
        // updateLB();

        if(count == 10) {
            writePose();
            writeAdditional();
            count = 1;
        }

        count++;

        closeO();

        pros::delay(10);
    }
}

void interruptLoop() {
    int count = 1;

    while (true) {
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        chassis.arcade(leftY, rightX * 0.9);

        updateIntake();
        updateClamp();
        //updateLadyPID();
        //updateLadyTask();
        // updateLB();

        if(count == 10) {
            writeInterruptPose();
            writeInterruptAdditional();
            count = 1;
        }

        count++;

        closeOInterrupt();

        pros::delay(10);
    }
}