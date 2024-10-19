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

// #include "autoSelect/selection.h"

// rd::Selector selector({
//    {"Blue AWP", &auton0},
//    {"Red AWP", &simple_auton},
//    {"Blue Rush" , &auton1},
//    {"Red Rush", &auton2}
//    {"Skills Run", &skills}
// });

pros::MotorGroup leftMotors({-3, -2, -16}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({19, 20, 18}, pros::MotorGearset::blue);

pros::Imu imu(13);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, 10, lemlib::Omniwheel::NEW_325, 450,
                              6 // 2 w/o traction
);

// lateral motion controller
lemlib::ControllerSettings linearController(9, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            6.55, // 48.905, //46.86273, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            2, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            0 // slew
);

// angular motion controller
lemlib::ControllerSettings angularController(1.33, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             10.2, // 38,//37.88, // 32.92, //40.5, // derivative gain (kD)
                                             0, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             2, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // slew

                                             // OLD VALUES OCT 6: P 4.05, D 34.86768
);
// sensors for odometry
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
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
    opticalInit();

    // lv_init();
    // selector::init();

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
            pros::lcd::print(4, "Color: %f", optical.get_hue());

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

ASSET(path_jerryio_txt);

void redSoloWP() {
    chassis.setPose(0, 0, 0);

    // mogo
    mogoClamp.set_value(true);
    chassis.turnToHeading(30, 1000, {.earlyExitRange = 3});
    // chassis.moveToPose(-5.69, -10.9, 27, 1000, {.forwards = false, .minSpeed = 100, .earlyExitRange = 40});
    //  chassis.moveToPoint(-11.18, -21.42, 1000, {.forwards = false, .maxSpeed = 50, .earlyExitRange = 20});
    chassis.moveToPoint(-10.63, -21.28, 1000,
                        {.forwards = false, .maxSpeed = 70, .earlyExitRange = 20}); // prev maxSpeed 50
    chassis.waitUntilDone();
    pros::delay(400); // prev delay 500
    mogoClamp.set_value(false);

    // first ring
    intake.move_voltage(-12000);
    intakeFirst.move_voltage(-12000);
    //pros::delay(300);
    chassis.turnToHeading(149, 1000);
    chassis.moveToPoint(1, -38.8, 1000, {.maxSpeed = 75});
    chassis.waitUntilDone();
    pros::delay(800);

    // second ring
    chassis.turnToHeading(32.22, 1000);

    chassis.moveToPoint(3.70, -33.55, 1000);
    pros::delay(800);

    chassis.turnToHeading(-19.26, 1000);
    intake.move_voltage(0);

    // drop mogo
    chassis.moveToPose(-30, 4.8, -90, 2000);
    intakeFirst.move_voltage(0);
    chassis.waitUntilDone();
    mogoClamp.set_value(true);

    // go to alliance
    chassis.moveToPoint(-35, 4.8, 1000);
    chassis.turnToHeading(180, 1000);
    chassis.waitUntilDone();
    pros::delay(100);
    mogoClamp.set_value(false);
    chassis.moveToPoint(
        -37.4, 15.07, 2000,
        {.forwards = false}); // chassis.moveToPoint(-37.96, 12, 2000, {.forwards = false}); //moves back
    // chassis.waitUntilDone();

    // score alliance
    pros::delay(200);
    intakeFirst.move_voltage(-12000);
    intake.move_voltage(-12000);
    pros::delay(150);
    lift.move_absolute(183, 40);
    pros::delay(1000); // TODO prev delay 1000

    // ladder
    intakeFirst.move_voltage(12000);
    intake.move_voltage(12000);
    chassis.moveToPose(-42.8, -16.1, -141.5, 7000);
    // chassis.moveToPoint(-41.21, -12.39, 1000);

    // chassis.turnToHeading(-173.2, 1000);

    // chassis.moveToPoint(-42.8, -16.6, 1000); //-38.49, -12.38, 1000
    lift.move_absolute(0, 40);
    intake.move_voltage(0);
    intakeFirst.move_voltage(0);

    // chassis.turnToHeading(221, 1000, {.earlyExitRange = 5}); REMOVAL OF MOGO MOVEMENT
    // chassis.waitUntilDone();
    // mogoClamp.set_value(true);

    // go to alliance stake
    // chassis.turnToHeading(270, 1000, {.earlyExitRange = 10});
    // chassis.moveToPose(-42.3, -2.39, -90, 2000);
    // chassis.waitUntilDone();
    // chassis.turnToHeading(182, 800, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE, .earlyExitRange = 5});
    // mogoClamp.set_value(false);
    // chassis.moveToPose(-39.15, 8, 180, 1000,
    //                    {.forwards = false}); // actual movement back to alliance stake BLINDCODE Y-=2, X-=1
    // chassis.waitUntilDone();
    // intake.move_voltage(-12000);
    // pros::delay(1000);

    // intake.move_voltage(0);
    // chassis.moveToPose(-39, -14.9, -156, 2000);

    // // go to ladder
    // //  chassis.moveToPoint(-39.89, -20.75, 1000, {. minSpeed = 75, .earlyExitRange = 20});
    // // chassis.turnToHeading(-162.1, 1000, {.earlyExitRange = 10}); //-148.7
    // intake.move_voltage(0);
}

void blueSoloWP() {
    chassis.setPose(0, 0, 0);
    mogoClamp.set_value(true);

    // mogo

    chassis.turnToHeading(-30, 800, {.earlyExitRange = 3});

    // chassis.moveToPose(-5.69, -10.9, 27, 1000, {.forwards = false, .minSpeed = 100, .earlyExitRange = 40});
    //  chassis.moveToPoint(-11.18, -21.42, 1000, {.forwards = false, .maxSpeed = 50, .earlyExitRange = 20});

    // chassis.moveToPose(10.94, -22.85, -30, 3000, {.forwards = false, .maxSpeed = 100, .earlyExitRange = 20});
    chassis.moveToPoint(12.3, -25.2, 1000, {.forwards = false, .maxSpeed = 75, .earlyExitRange = 20});

    chassis.waitUntilDone();
    pros::delay(400);
    mogoClamp.set_value(false);

    // first ring
    intake.move_voltage(-12000);
    intakeFirst.move_voltage(-12000);
    pros::delay(300);

    chassis.turnToHeading(-150, 1000);

    chassis.moveToPoint(2.5, -47.8, 3000, {.maxSpeed = 75});

    chassis.waitUntilDone();
    pros::delay(500);

    // // //pros::delay(500);

    // //go back for second ring

    // second ring adventure
    chassis.turnToHeading(-24.5, 1000);
    chassis.moveToPoint(-2, -45.1, 3000);
    pros::delay(650);

    chassis.turnToHeading(35, 1000);
    intake.move_voltage(0);

    // drop the mogo
    chassis.moveToPose(25, -5, 90, 2000);
    chassis.waitUntilDone();
    intakeFirst.move_voltage(0);
    mogoClamp.set_value(true);

    // go to alliance
    chassis.moveToPoint(40.25, -5, 1000); // BLINDCODED FROM HERE ON
    chassis.turnToHeading(185, 1000);
    chassis.waitUntilDone();
    pros::delay(100);
    mogoClamp.set_value(true);

    chassis.moveToPoint(44.3, 0.55, 3000, {.forwards = false});

    pros::delay(200);
    intakeFirst.move_voltage(-12000);
    intake.move_voltage(-12000);
    pros::delay(150);
    lift.move_absolute(183, 40);
    pros::delay(1000); // TODO prev delay 1000

    intakeFirst.move_voltage(12000);
    intake.move_voltage(12000);
    chassis.moveToPose(44.3, -27, 142, 7000);
    lift.move_absolute(0, 40);
    intake.move_voltage(0);
    intakeFirst.move_voltage(0);
}

void redMogo() {
    double liftPosition = lift.get_position();

    mogoClamp.set_value(true);

    // mogo rush
    chassis.moveToPoint(0, -28, 1000, {.forwards = false});

    chassis.moveToPose(5.6, -46.4, -29.3, 2000, {.forwards = false, .maxSpeed = 45});

    chassis.waitUntilDone();

    mogoClamp.set_value(false);

    pros::delay(100);
    intake.move_voltage(-12000);
    pros::delay(800);
    chassis.turnToHeading(14.3, 800);
    // chassis.moveToPoint(6.98, -38.98, 1000);

    intakeFirst.move_velocity(-600);

    chassis.moveToPoint(8.5, -32.9, 1000);

    pros::delay(1000);

    // intakeFirst.move_velocity(600);

    chassis.turnToHeading(99.8, 1000);

    chassis.waitUntilDone();

    pros::delay(1000);

    chassis.moveToPoint(26.8, -37.7, 5000, {.maxSpeed = 30});

    intakeFirst.move_velocity(0);
    // mogoClamp.set_value(true);
}

void blueMogo() {
    double liftPosition = lift.get_position();

    mogoClamp.set_value(true);

    // mogo rush
    chassis.moveToPoint(0, -28, 3000, {.forwards = false});


    chassis.moveToPose(-5.9, -48.4, 30, 4000, {.forwards = false, .maxSpeed = 45});
    // chassis.moveToPose(-7.3, -45.6, 30, 2000, {.forwards = false, .maxSpeed = 45});

    chassis.waitUntilDone();

    mogoClamp.set_value(false);

    pros::delay(100);
    intake.move_voltage(-12000);
    pros::delay(1100);
    chassis.turnToHeading(-14.3, 800);
    // chassis.moveToPoint(6.98, -38.98, 1000);

    intakeFirst.move_velocity(-600);

    chassis.moveToPoint(-8.5, -32.9, 3000);

    pros::delay(1500);

    // intakeFirst.move_velocity(600);

    chassis.turnToHeading(-131.4, 2000);

    chassis.waitUntilDone();

    chassis.moveToPose(-31.1, -49.5, -138.4, 5000, {.lead = 0.1, .maxSpeed = 35});

    intakeFirst.move_velocity(0);
    // mogoClamp.set_value(true);
    
    
}

void skills() {
    double liftPosition = lift.get_position();

    // score alliance
    lift.move_absolute(183, 40);
    intake.move_voltage(-12000);
    pros::delay(750);

    // ring 1
    lift.move_absolute(liftPosition, -60);
    intakeFirst.move_velocity(-600);
    intake.move_voltage(0);

    chassis.moveToPose(29.8, 43.8, 42.5, 5000); // chassis.moveToPose(24.2, 36.14, 43.47, 2000);
    chassis.waitUntilDone();

    // mogo

    mogoClamp.set_value(true);

    chassis.moveToPose(28, 26.4, 30, 2000, {.forwards = false, .lead = 0.5, .maxSpeed = 50});

    chassis.waitUntilDone();
    mogoClamp.set_value(false);

    pros::delay(150);
    intake.move_voltage(-12000);

    // rings 2, 3, 4
    chassis.moveToPose(44.4, 39.8, 50.97, 2000); // theta: 416.28
    pros::delay(2000);
    chassis.turnToHeading(174, 800);
    chassis.moveToPoint(50.25, 14.34, 4000, {.maxSpeed = 45});
    chassis.waitUntilDone();
    pros::delay(500);

    // ring 5
    chassis.moveToPoint(46.66, 34.3, 1000, {.forwards = false, .maxSpeed = 70});
    pros::delay(1000);
    chassis.turnToHeading(133.5, 800);
    chassis.moveToPoint(59.7, 26.1, 1000, {.maxSpeed = 70});
    pros::delay(2300);

    // mogo to corner
    chassis.turnToHeading(-28.4, 800, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});

    chassis.moveToPoint(58, 16.9, 1000, {.forwards = false});
    chassis.waitUntilDone();
    pros::delay(300);
    mogoClamp.set_value(true);
    pros::delay(300);

    // position forward
    chassis.moveToPoint(55, 24.2, 1000);
    chassis.turnToHeading(90, 800);

    // move to second mogo
    chassis.moveToPose(
        -8, 25.8, 90, 3000,
        {.forwards = false,
         .maxSpeed = 85}); // chassis.moveToPose(-11.19, 21.55, 90, 3000, {.forwards = false, .maxSpeed = 95});
    /*
    chassis.moveToPose(
        -15.3, 27.2, 90, 2000,
        {.forwards = false,
         .maxSpeed = 10}); // chassis.moveToPose(-16.6, 21.55, 90, 2000, {.forwards = false, .maxSpeed = 15});
    chassis.waitUntilDone();
    pros::delay(500);

    // clamp onto second mogo
    mogoClamp.set_value(false);
    pros::delay(150);

    chassis.moveToPoint(-22, 27.2, 3000, {.forwards = false});

    // first ring on second side

    intakeFirst.move_velocity(-600);
    intake.move_voltage(-12000);

    chassis.turnToHeading(0, 2000);
    chassis.moveToPoint(-26.4, 49, 3000, {.maxSpeed = 35}); // change coord to be faster
    pros::delay(1750);

    // 2nd ring on second side
    chassis.turnToHeading(-90, 2000);
    chassis.moveToPoint(-47.6, 47, 3000, {.maxSpeed = 40}); // change, slow down

    pros::delay(1750);

    // 3rd + 4th ring
    chassis.turnToHeading(-180, 2000);
    chassis.moveToPoint(-47.5, 16.3, 4000, {.maxSpeed = 35}); // possibly faster?
    chassis.waitUntilDone();
    pros::delay(1500);

    // GETTTTTTTTT NEWWW COORDDDDSSS FORRRRRRR THISSSSSSSSSS @PROGGGGGGGGG
    chassis.moveToPoint(-45.5, 36.6, 1000, {.forwards = false, .maxSpeed = 40});
    pros::delay(300);
    chassis.turnToHeading(-123.83, 1000);

    // 5th ring

    chassis.moveToPoint(-53.6, 26.6, 3000, {.forwards = true});
    chassis.waitUntilDone();
    pros::delay(1750);

    chassis.turnToHeading(20.9, 1000);
    pros::delay(2000);

    chassis.moveToPoint(-63.73, 13.6, 3000, {.forwards = false});
    chassis.waitUntilDone();

    mogoClamp.set_value(true);
    
    //second half!!!
    intake.move_voltage(0);

    //mogo
    chassis.moveToPose(-45.86, 63.36, 0, 2000);
    chassis.moveToPose(-29.5, 85.42, 39.6, 2000);
    chassis.turnToHeading(180, 2000);
    chassis.moveToPose(0, 114.57, 270, 4000, {.forwards = false, .lead =0.4, .maxSpeed = 50});
    chassis.waitUntilDone();
    
    mogoClamp.set_value(false);
    pros::delay(500);
    intake.move_velocity(-12000);
    pros::delay(750);

    //move to first ring
    
    chassis.moveToPose(-42.3, 123.7, 296.2, 1000, {.lead = 0.2});
    chassis.waitUntilDone();
    pros::delay(1000);
    chassis.moveToPoint(-23.87, 114.55, 1000, {.forwards = false});
    
    //second and third ring
    chassis.moveToPose(-41.5, 113, 270, 1000, {.maxSpeed = 45});
    chassis.moveToPoint(-51.2, 113, 1000, {.maxSpeed = 45});
*/

}

void autonomous() {
    // with selector

    /*if (selector::auton == 1) { redSoloWP(); }
    if (selector::auton == 2) { blueSoloWP(); }
    if (selector::auton == 3) { redMogo(); }
    if (selector::auton == -1) { blueMogo(); }
    if (selector::auton == 0) { skills(); }*/

    // without selector

    // redSoloWP(); // red alliance solo AWP

    // blueSoloWP(); // blue alliance solo AWP

    // redMogo();  //red alliance mogo rush

     //blueMogo(); // blue alliance mogo rush

    // lift.move_absolute(183, 40);

    skills(); // prog skills

    // chassis.follow(path_jerryio_txt, 15, 3000);

    // chassis.turnToHeading(180, 10000);

    // chassis.moveToPoint(0, 24, 2000);
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
        //  // log position telemetry
        //  lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
        //  get joystick positions;
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the chassis with curvature drive

        chassis.arcade(leftY, rightX * 0.8);
        // chassis.arcade(leftY, rightX, false, 1);

        pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
        pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
        pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
        pros::lcd::print(3, "Lift: %f", lift.get_position()); // lift encoder
        pros::lcd::print(4, "Color: %f", optical.get_hue());

        /*std::vector<double> left = leftMotors.get_position_all();
        std::vector<double> right = rightMotors.get_position_all();
        pros::lcd::print(5, "LeftF Encoders: %f", left[0]);
        pros::lcd::print(6, "LeftM Encoders: %f", left[1]);
        pros::lcd::print(7, "LeftB Encoders: %f", left[2]);
        pros::lcd::print(2, "RightF Encoders: %f", right[0]);
        pros::lcd::print(3, "RightM Encoders: %f", right[1]);
        pros::lcd::print(4, "RightB Encoders: %f", right[2]);*/

        // log position telemetry
        lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());

        updateIntake();
        updateIntakeFirst();
        updateClamp();
        updateIntakeClamp();
        updateLift();
        // sortTest();
        updateColorToggle();
        colorSort();

        if (sortState == 0) {
            controller.set_text(0, 0, "no sort   ");
        } else if (sortState == 1) {
            controller.set_text(0, 0, "scores blue");
        } else if (sortState == 2) {
            controller.set_text(0, 0, "scores red ");
        }

        // resetIntake();
        // stepIntake();
        // allianceStake();
        pros::delay(10);
    }
}
