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
            pros::lcd::print(4, "Hooks: %f", intakeState);
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

    // move to alliance (push rings) and turn to allign with alliance
    intakeFirst.move_velocity(0);
    // chassis.moveToPose(-24.5, -15.7, 243.33, 3000, {.earlyExitRange = 5});
    chassis.moveToPose(-27.5, -13.5, 243.33, 3000, {.earlyExitRange = 5});
    chassis.turnToHeading(153, 1000);
    mogoClamp.set_value(false);

    //move backwards to alliance stake
    chassis.moveToPose(-29.12, -4.96, 153, 2000, {.forwards = false});
    intake.move_voltage(-12000);

    //score on alliance stake with lift
    lift.move_absolute(183, 40);
    pros::delay(1500);
    intake.move_voltage(0);
    lift.move_absolute(liftPosition, -60);
    
    //go to ladder
    intakeFirst.move_voltage(12000);
    chassis.moveToPose(-19.98, -31.39, 164, 3000, {.maxSpeed = 50});
}

void redMogo() {
    double liftPosition = lift.get_position();

    mogoClamp.set_value(true);

    // mogo rush
    chassis.moveToPoint(0, -28, 1000, {.forwards = false});
    chassis.moveToPose(5.47, -44.78, -34.22, 2000, {.forwards = false});

    chassis.waitUntilDone();

    mogoClamp.set_value(false);
    pros::delay(100);
    intake.move_voltage(-12000);
    pros::delay(300);
    chassis.turnToHeading(21.03, 800);
    chassis.moveToPoint(6.98, -38.98, 1000);

    intakeFirst.move_velocity(-600);

    chassis.moveToPoint(11.29, -27.50, 1000);

    pros::delay(450);

    intakeFirst.move_velocity(600);

    chassis.turnToHeading(124.38, 1000);

    chassis.waitUntilDone();

    pros::delay(600);

    chassis.moveToPoint(28.47, -36.19, 3000, {.maxSpeed = 45});

    mogoClamp.set_value(true);
}

void blueSoloWP() {
    double liftPosition = lift.get_position();

    // theta = 32.26

    // go to mogo
    mogoClamp.set_value(true);
    chassis.moveToPoint(0, -16, 2000, {.forwards = false});
    chassis.moveToPoint(0, -21, 3000, {.forwards = false, .maxSpeed = 50});
    chassis.waitUntilDone();

    // clamp
    mogoClamp.set_value(false);
    intake.move_voltage(-12000);
    pros::delay(150);

    // go to first ring
    chassis.moveToPose(-11.29, -17.44, -80.95, 2000);
    intakeFirst.move_velocity(-600);
    pros::delay(250);

    // go away from first ring
    chassis.moveToPose(-16.3, -31.6, -142.89, 2000);
    chassis.waitUntilDone(); // could be less

    // release mogo
    pros::delay(250);
    intake.move_voltage(0);
    chassis.moveToPoint(13.2, -10.96, 2000, {.forwards = false, .maxSpeed = 100});
    chassis.waitUntilDone();
    chassis.turnToHeading(-238.5, 800);
    chassis.waitUntilDone();
    mogoClamp.set_value(true);
    chassis.waitUntilDone();
    pros::delay(500);
    intakeFirst.move_velocity(0);
    // chassis.moveToPose(-24.5, -15.7, 243.33, 3000, {.earlyExitRange = 5});

    // move to alliance stake
    // chassis.moveToPose(39.3, -14.4, -240.5, 3000);
    chassis.moveToPose(37.5, -18.2, -240.5, 3000);
    chassis.turnToHeading(207, 1000);
    mogoClamp.set_value(false);

    // score alliance stake

    chassis.moveToPoint(44.2, -13.1, 2000, {.forwards = false});
    chassis.waitUntilDone();
    intake.move_voltage(-12000);
    lift.move_absolute(183, 40);
    pros::delay(1500);
    intake.move_voltage(0);
    lift.move_absolute(liftPosition, -60);

    // move to ladder

    intakeFirst.move_voltage(12000); // outtake to repel rings in the way of ladder
    chassis.moveToPose(28.1, -31.9, 220.6, 3000, {.maxSpeed = 38});
}

void blueMogo() {
    double liftPosition = lift.get_position();

    mogoClamp.set_value(true);

    // mogo rush
    chassis.moveToPoint(0, -28, 1000, {.forwards = false});
    chassis.moveToPose(-5.47, -44.78, 34.22, 2000, {.forwards = false});

    chassis.waitUntilDone();

    mogoClamp.set_value(false);
    pros::delay(100);
    intake.move_voltage(-12000);
    pros::delay(300);
    chassis.turnToHeading(-21.03, 800);
    chassis.moveToPoint(-6.98, -38.98, 1000);

    intakeFirst.move_velocity(-600);

    chassis.moveToPoint(-11.29, -27.50, 1000);

    pros::delay(450);

    intakeFirst.move_velocity(600);

    chassis.turnToHeading(-124.38, 1000);

    chassis.waitUntilDone();

    pros::delay(600);

    chassis.moveToPoint(-33.81, -46.4, 3000, {.maxSpeed = 45});

    mogoClamp.set_value(true);
}

void skills() {
    double liftPosition = lift.get_position();

    //score alliance
    lift.move_absolute(183, 40);
    intake.move_voltage(-12000);
    pros::delay(750);

    //ring 1
    lift.move_absolute(liftPosition, -60);
    intakeFirst.move_velocity(-600);
    intake.move_voltage(0);
    chassis.moveToPose(30.19, 43.45, 42.34, 2000); //chassis.moveToPose(24.2, 36.14, 43.47, 2000);
    chassis.waitUntilDone();

    //mogo
    mogoClamp.set_value(true);
    chassis.moveToPose(27.4, 25.1, 28.67, 2000, {.forwards = false, .maxSpeed = 50});

    chassis.waitUntilDone();
    pros::delay(50);
    mogoClamp.set_value(false);
    pros::delay(50);
    intake.move_voltage(-12000);

    //rings 2, 3, 4
    chassis.moveToPose(44.4, 37.1, 50.97, 2000); //theta: 416.28
    pros::delay(1000);
    chassis.turnToHeading(180, 800);
    chassis.moveToPoint(47.23, 12.5, 2000, {.maxSpeed = 35});
    chassis.waitUntilDone();


    //ring 5
    chassis.moveToPoint(46.66, 33.87, 1000, {.forwards = false});
    chassis.turnToHeading(135.8, 800);
    chassis.moveToPoint(54.18, 26.94, 1000);
    pros::delay(2000);

    //mogo to corner
    chassis.turnToHeading(-18.57, 800, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
    chassis.moveToPoint(58.14, 17.1, 1000, {.forwards = false});
    chassis.waitUntilDone();
    pros::delay(300);
    mogoClamp.set_value(true);


    //position forward
    chassis.moveToPoint(54.64, 21.55, 1000);
    chassis.turnToHeading(90, 800);

    //move to second mogo
    /*chassis.moveToPoint(-11.19, 21.55, 3000, {.forwards = false, .maxSpeed = 95}); //chassis.moveToPose(-11.19, 21.55, 90, 3000, {.forwards = false, .maxSpeed = 95});
    chassis.moveToPoint(-16.6, 21.55, 2000, {.forwards = false, .maxSpeed = 15}); //chassis.moveToPose(-16.6, 21.55, 90, 2000, {.forwards = false, .maxSpeed = 15});
    chassis.waitUntilDone();

    //clamp onto second mogo
    mogoClamp.set_value(false);


    //chassis.moveToPoint(-24.69, 26.18, 3000);

    /*chassis.turnToHeading(-18, 2000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
    chassis.moveToPoint(-29, 48.5, 3000);

    chassis.turnToHeading(270, 2000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE} );
    chassis.moveToPoint(-50, 49, 3000);
    
    chassis.turnToHeading(180, 2000);
    chassis.moveToPoint(-50, 12.5, 3000, {.maxSpeed = 35});
    
    chassis.moveToPoint(-50, 33.87, 1000, {.forwards = false});
    chassis.turnToHeading(219, 800);*/
    
}

void autonomous() {
    // redSoloWP(); // red alliance solo AWP

    //blueSoloWP(); // blue alliance solo AWP

    //redMogo();  //red alliance mogo rush

    //blueMogo();  //blue alliance mogo rush

    skills();  //prog skills
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
        chassis.arcade(leftY, rightX*0.8);
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
