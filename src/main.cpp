#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "ports.hpp"
#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "lift.hpp"
#include "intakeFirst.hpp"
#include "intakePiston.hpp"

pros::MotorGroup leftMotors({-3, -1, -16}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({19, 20, 18}, pros::MotorGearset::blue);

pros::Imu imu(2);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, 10, lemlib::Omniwheel::NEW_325, 425,
                              6 // 2 w/o traction
);

// lateral motion controller
lemlib::ControllerSettings linearController(13.5, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            30.43, // 22, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            0 // slew
);

// angular motion controller
lemlib::ControllerSettings angularController(4.6, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             37.38, // derivative gain (kD)
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

void redSoloWP() {
    double liftPosition = lift.get_position();

    // theta = 32.26
    // go to mogo and clamp
    mogoClamp.set_value(true);
    chassis.moveToPoint(0, -16, 2000, {.forwards = false});
    chassis.moveToPoint(0, -21, 3000, {.forwards = false, .maxSpeed = 50});
    chassis.waitUntilDone();

    mogoClamp.set_value(false);
    intake.move_voltage(-12000);
    pros::delay(150);

    // go to first ring
    // chassis.turnToHeading(78.2, 800);
    // chassis.moveToPose(11.04, -18.63, 82.26, 2000);
    chassis.moveToPose(11.29, -17.44, 76.95, 2000);
    intakeFirst.move_velocity(-600);
    // pros::delay(250;0); //could be less IF  ERROR UNCOMMENT THIS
    //  LINE <---------------------

    chassis.moveToPose(20.64, -26.97, 125.9, 2000);

    chassis.waitUntilDone(); // could be less

    // go to point and release mogo
    pros::delay(250);
    intake.move_voltage(0);
    chassis.moveToPoint(-5.83, -3.54, 2000, {.forwards = false});
    chassis.waitUntilDone();
    chassis.turnToHeading(249, 800);

    chassis.waitUntilDone();
    mogoClamp.set_value(true);

    chassis.waitUntilDone();
    pros::delay(500);

    // move to alliance
    intakeFirst.move_velocity(0);
    // chassis.moveToPose(-24.5, -15.7, 243.33, 3000, {.earlyExitRange = 5});
    chassis.moveToPose(-27.5, -13.5, 243.33, 3000, {.earlyExitRange = 5});
    chassis.turnToHeading(153, 1000);
    mogoClamp.set_value(false);
    chassis.moveToPose(-29.12, -4.96, 153, 2000, {.forwards = false});
    intake.move_voltage(-12000);
    lift.move_absolute(183, 40);
    pros::delay(1500);
    intake.move_voltage(0);
    lift.move_absolute(liftPosition, -60);

    intakeFirst.move_voltage(12000);
    chassis.moveToPose(-19.98, -31.39, 164, 3000, {.minSpeed = 75});
}

void redSoloWPMogo() {
    double liftPosition = lift.get_position();

    mogoClamp.set_value(true);

    // mogo rush
    chassis.moveToPoint(0, -24, 800, {.forwards = false});
    chassis.moveToPose(5.4, -46, -25, 1000, {.forwards = false});

    chassis.waitUntilDone();
    mogoClamp.set_value(false);
    pros::delay(300);

    // getting the rings
    chassis.turnToHeading(18.64, 800);
    chassis.moveToPoint(6.25, -39.98, 800);
    intakeFirst.move_velocity(-600);
    intake.move_voltage(-12000);

    // going to the ladder
    chassis.turnToHeading(116.69, 800);
    chassis.moveToPoint(20.6, -42.6, 800);

    pros::delay(2000);

    mogoClamp.set_value(false);
    // mogoClamp.set_value(false); 116.69 20.6 -42.6
}

void blueSoloWP() {
    double liftPosition = lift.get_position();

    // theta = 32.26
    // go to mogo and clamp
    mogoClamp.set_value(true);
    chassis.moveToPoint(0, -16, 2000, {.forwards = false});
    chassis.moveToPoint(0, -21, 3000, {.forwards = false, .maxSpeed = 50});
    chassis.waitUntilDone();

    mogoClamp.set_value(false);
    intake.move_voltage(-12000);

    pros::delay(150);

    chassis.turnToHeading(-76.82, 800);

    chassis.moveToPoint(-9.47, -22.93, 1000);

    intakeFirst.move_velocity(-600);
    // // pros::delay(250;0); //could be less IF  ERROR UNCOMMENT THIS
    // //  LINE <---------------------

    chassis.moveToPose(-16.3, -31.6, -132.89, 2000);

    chassis.waitUntilDone(); // could be less

    // go to point and release mogo
    pros::delay(250);
    intake.move_voltage(0);
    chassis.moveToPoint(13.2, -10.96, 2000, {.forwards = false});
    chassis.waitUntilDone();

    // turning to alliance
    chassis.turnToHeading(-238.5, 800);
    chassis.waitUntilDone();
    mogoClamp.set_value(true);
    chassis.waitUntilDone();
    pros::delay(500);
    chassis.moveToPoint(37.97, -16.5, 1000);

    // face alliance stake
    chassis.turnToHeading(-149.37, 800);
    intakeFirst.move_velocity(0);
    mogoClamp.set_value(false);

    // going back into alliance stake
    chassis.moveToPose(39.59, -13.97, -149.37, 800, {.forwards = false});

    // scoring on alliance
    intake.move_voltage(-12000);
    lift.move_absolute(183, 40);
    pros::delay(1500);
    intake.move_voltage(0);
    lift.move_absolute(liftPosition, -60);

    // moving to ladder
    chassis.moveToPose(22.27, -32.37, -136.4, 3000, {.minSpeed = 75});
}

void autonomous() {
    // redSoloWP(); // red alliance solo AWP

    blueSoloWP(); // blue alliance solo AWP

    // redSoloWPMogo();
}

/**
 * Runs in driver control
 */

void opcontrol() {
    // controller
    // loop to continuously update motors
    while (true) {
        // pros::lcd::set_text(0, "X: %f", chassis.getPose().x); // x
        // pros::lcd::set_text(1, "Y: %f", chassis.getPose().y); // y
        // pros::lcd::set_text(2, "Theta: %f", chassis.getPose().theta); // heading
        // // log position telemetry
        // lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
        // get joystick positions
        pros::lcd::set_text(0, std::to_string(chassis.getPose().x));
        pros::lcd::set_text(1, std::to_string(chassis.getPose().y));
        pros::lcd::set_text(2, std::to_string(chassis.getPose().theta));
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the chassis with curvature drive
        chassis.arcade(leftY, rightX);
        // chassis.arcade(leftY, rightX, false, 1);
        updateIntake();
        updateIntakeFirst();
        updateClamp();
        updateIntakeClamp();
        updateLift();
        resetIntake();
        stepIntake();
        // allianceStake();
        pros::delay(10);
    }
}
