#include "globals.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "magic.hpp"
#include <cmath>
#include "ladybrown.hpp"


//Note: try moving all this back to main and see if initializing chassis works in initialize
pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::MotorGroup leftMotors({-16, -17, -12}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({18, 15, 11}, pros::MotorGearset::blue);

// pros::Rotation vertical(-1);
// pros::Rotation horizontal(10); //TODO: check later

pros::Imu imu(3);

// pros::Rotation verticalEnc(1);

// lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_275, 0.125);

// pros::Rotation horizontalEnc(3);

// lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_275, -4.625); //2.75

// lemlib::TrackingWheel verticalTracker(&vertical, lemlib::Omniwheel::NEW_275, 0.125);
// lemlib::TrackingWheel horizontalTracker(&horizontal, lemlib::Omniwheel::NEW_275, -2.5);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, 12.5, lemlib::Omniwheel::NEW_275, 450,
                              8 //13.4 trackwidth artificial
);

// lateral motion controller
lemlib::ControllerSettings linearController(7.3, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            1, // 48.905, //46.86273, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            2, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            0 // slew
);

// angular motion controller
lemlib::ControllerSettings angularController(7.5, // proportional gain (kP) 4.6 //TODO: maybe tune more!
                                             0, // integral gain (kI)
                                             58.3, // 38,//37.88, // 32.92, //40.5, // derivative gain (kD) 2
                                             0, // anti windup
                                             0.5, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             1, // large error range, in degrees
                                             1500, // large error range timeout, in milliseconds
                                             0 // slew

                                             // OLD VALUES OCT 6: P 4.05, D 34.86768
);
// sensors for odometry
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null //TODO: what the actual flip
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
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

void interruptLoop() {
    int count = 1; //TODO: check count and segCount logic
    int segCount = 1;

    while (true) {
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        chassis.arcade(leftY, rightX * 0.9);

        updateIntake();
        updateColorSort();
        updateClamp();
        updateDoink();
        updateLB();
        runLB();

        if(count == 10) {
            writeInterruptPose();
            writeInterruptAdditional();
            count = 1;
            fileInterrupt.flush();
            fileInterruptTwo.flush();
        }

        if(segCount == 100) {
            section++;
        }

        count++;
        segCount++;

        closeOInterrupt();

        pros::delay(10);
    }
}