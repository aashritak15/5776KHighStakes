#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "ports.hpp"
#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "lift.hpp"
#include "intakeFirst.hpp"
#include "intakePiston.hpp"
#include "autons.hpp"


//#include "autoSelect/selection.h"

// rd::Selector selector({
//    {"Blue AWP", &auton0},
//    {"Red AWP", &simple_auton},
//    {"Blue Rush" , &auton1},
//    {"Red Rush", &auton2}
//    {"Skills Run", &skills}
// });

pros::MotorGroup leftMotors({-3, -1, -16}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({19, 20, 18}, pros::MotorGearset::blue);

pros::Imu imu(4);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, 10, lemlib::Omniwheel::NEW_325, 425,
                              6 // 2 w/o traction
);

// lateral motion controller
lemlib::ControllerSettings linearController(20, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            0, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            0 // slew
);

// angular motion controller
lemlib::ControllerSettings angularController(3.6, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             32.92, //40.5, // derivative gain (kD)
                                             0, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             2, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // slew
);

// sensors for odometry
lemlib::OdomSensors sensors(nullptr, nullptr, nullptr, nullptr,
                            &imu // inertial sensor
);

// tanish driver functions:

lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors
    clampInit();
    intakeInnit();
    liftInit();
    intakeClampInit();

    //lv_init();
    //selector::init();

    // the default rate is 50. however, if you need to change the rate, you
    // can do the following.
    // lemlib::bufferedStdout().setRate(...);
    // If you use bluetooth or a wired connection, you will want to have a rate of 10ms

    // for more information on how the formatting for the loggers
    // works, refer to the fmtlib docs

    // thread to for brain screen and position logging
    pros::Task screenTask([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            pros::lcd::print(3, "Lift: %f", lift.get_position()); // lift encoder
            // pros::lcd::print(4, "Color: %f", optical.get_hue());
            // log position telemetry
            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
            // delay to save resources
            pros::delay(50);
        }
    });
}

/**
 * Runs while the robot is disabled
 */
void disabled() {}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}

/**
 * Runs during auto
 *
 * This is an example autonomous routine which demonstrates a lot of the features LemLib has to offer
 */


void autonomous() {
    // with selector

    /*if (selector::auton == 1) { redSoloWP(); }
    if (selector::auton == 2) { blueSoloWP(); }
    if (selector::auton == 3) { redMogo(); }
    if (selector::auton == -1) { blueMogo(); }
    if (selector::auton == 0) { skills(); }*/

    // without selector

    //redSoloWP(); // red alliance solo AWP

    // blueSoloWP(); // blue alliance solo AWP

    // redMogo();  //red alliance mogo rush

    // blueMogo();  //blue alliance mogo rush

    //skills(); // prog skills

    chassis.turnToHeading(90, 1000);
    //chassis.moveToPoint(0, 24, 1000);
}

/**
 * Runs in driver control
 */

void opcontrol() {
    // controller
    // loop to continuously update motors
    while (true) {
        //pros::lcd::set_text(0, "X: %f", chassis.getPose().x); // x
        //pros::lcd::set_text(1, "Y: %f", chassis.getPose().y); // y
        //pros::lcd::set_text(2, "Theta: %f", chassis.getPose().theta); // heading
        // // log position telemetry
        // lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
        // get joystick positions;
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the chassis with curvature drive
        chassis.arcade(leftY, rightX * 0.8);
        // chassis.arcade(leftY, rightX, false, 1);
        updateIntake();
        updateIntakeFirst();
        updateClamp();
        updateIntakeClamp();
        updateLift();
        // resetIntake();
        // stepIntake();
        // allianceStake();
        pros::delay(10);
    }
}
