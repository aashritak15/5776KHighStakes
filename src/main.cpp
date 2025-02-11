#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "ladybrown.hpp"
#include "magic.hpp"
#include <cmath>

void initialize() {
    pros::lcd::initialize();

    clampInit();
    doinkInit();
    intakeInit();
    ladyBrownInit();
    opticalInit();

    // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    pros::Task screenTask([&]() {
        while (true) {
            // //* line 7 reserved for rerun states.
            // std::cout<<chassis.getPose().x<<", "<<chassis.getPose().y<<", "<<chassis.getPose().theta<<"\n";

            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // pros::lcd::print(3, "Color: %f", optical.get_hue());
            pros::lcd::print(4, "LB Rot Sensor: %i", lbRotation.get_position());
            // pros::lcd::print(5, "Tracking: %i", horizontalEnc.get_position());

            // pros::lcd::print(3, "Rotation (Lift): %i", rotationSensor.get_position()); // lift encoder
            // pros::lcd::print(4, "Intake State: %f", intakeState);

            // std::vector<double> left = leftMotors.get_position_all();
            // std::vector<double> right = rightMotors.get_position_all();
            // pros::lcd::print(3, "LeftF Encoders: %f", left[0]);
            // pros::lcd::print(4, "LeftM Encoders: %f", left[1]);
            // pros::lcd::print(5, "LeftB Encoders: %f", left[2]);
            // pros::lcd::print(6, "RightF Encoders: %f", right[0]);
            // pros::lcd::print(7, "RightM Encoders: %f", right[1]);
            // pros::lcd::print(8, "RightB Encoders: %f", right[2]);
            // pros::lcd::print(7 "Color: %f", optical.get_hue());

            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());

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

ASSET(autonomous_txt); // TODO: add std functionality
ASSET(extra_txt);

void blueSoloWP() {
    chassis.moveToPose(3.4, 5.3, 33.3, 1000, {.maxSpeed = 30});
    chassis.waitUntilDone();
    pros::delay(100);
    ladyBrown.move_absolute(915, 100); // change to score angle
    pros::delay(500);
    ladyBrown.move_absolute(0, 100);
    pros::delay(100);

    // mogo
    chassis.turnToHeading(21.7, 1000);
    chassis.moveToPoint(-7.4, -24.9, 3000, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    mogoClamp.set_value(true);
    pros::delay(250);

    // center rings
    chassis.turnToHeading(-140.7, 2000);
    intake.move_voltage(-12000);
    chassis.moveToPoint(-24.5, -46.4, 2000);
    chassis.turnToHeading(-116, 1000);
    chassis.moveToPoint(-29.8, -48, 2000);
    chassis.waitUntilDone();
    pros::delay(1000);

    // middle ring
    chassis.moveToPoint(-10.6, -44.7, 2000, {.forwards = false});
    chassis.turnToHeading(-60, 1000);
    chassis.moveToPoint(-24.8, -34.7, 2000);
    pros::delay(1750);

    // //ladder
    // chassis.turnToHeading(-229.6, 1000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
    // chassis.moveToPoint(-12.7, -48.3, 2000, {.maxSpeed = 50});
    // chassis.waitUntilDone();
    // intake.move_voltage(0);

    // elims
    chassis.moveToPose(36, -15, 90, 2000);
    chassis.waitUntilDone();
    intake.move_voltage(0);
}

void redSoloWP() {
    // inversed blue
    chassis.moveToPose(-3.4, 5.3, -33.3, 1000, {.maxSpeed = 30});
    chassis.waitUntilDone();
    pros::delay(100);
    ladyBrown.move_absolute(915, 100); // change to score angle
    pros::delay(500);
    ladyBrown.move_absolute(0, 100);
    pros::delay(100);

    // mogo
    chassis.turnToHeading(-21.7, 1000);
    chassis.moveToPoint(7.4, -24.9, 3000, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    mogoClamp.set_value(true);
    pros::delay(250);

    // center rings
    chassis.turnToHeading(144.1, 2000);
    intake.move_voltage(-12000);
    chassis.moveToPoint(22.2, -49.3, 2000);
    pros::delay(500);
    chassis.turnToHeading(114.6, 1000);
    chassis.moveToPoint(28.5, -52.1, 2000);
    chassis.waitUntilDone();
    pros::delay(1000);

    // middle ring
    chassis.moveToPoint(10.6, -44.7, 2000, {.forwards = false});
    chassis.turnToHeading(60, 1000);
    chassis.moveToPoint(24.8, -34.7, 2000);
    pros::delay(1750);

    // //ladder
    // chassis.turnToHeading(229.6, 1000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
    // chassis.moveToPoint(12.7, -48.3, 2000, {.maxSpeed = 50});
    // chassis.waitUntilDone();
    // intake.move_voltage(0);

    // elims
    intake.move_voltage(0);
    chassis.moveToPose(-36, -15, -90, 2000);
    chassis.waitUntilDone();

    // old red
    //  ladyBrown.move_absolute(915, 100); // change to score angle
    //  pros::delay(500);
    //  ladyBrown.move_absolute(0, 100);
    //  pros::delay(100);

    // // mogo
    // // chassis.turnToHeading();
    // // chassis.moveToPoint(-4.7, -29.9, 3000, {.forwards = false, .maxSpeed = 80});
    // chassis.moveToPoint(-6.45, -34.1, 2000, {.forwards = false, .maxSpeed = 80});
    // chassis.waitUntilDone();
    // // pros::delay(100);
    // mogoClamp.set_value(true);
    // pros::delay(250);

    // // center rings
    // chassis.turnToHeading(166, 2000);
    // intake.move_voltage(-12000);

    // chassis.moveToPoint(-4.079, -61.039, 3000, {.forwards = true, .maxSpeed = 80});

    // chassis.turnToHeading(135, 2000);

    // chassis.moveToPoint(-0.7, -64.4, 3000, {.minSpeed = 80});
    // chassis.waitUntilDone();
    // pros::delay(1000);

    // // middle ring
    // //-9.8, -53.2, 146
    // chassis.moveToPoint(-16.2, -47.9, 2000, {.forwards = false}); // get new point in between so it doesnt cross line
    // chassis.turnToHeading(98, 1000);
    // chassis.moveToPoint(2.2, -46.4, 2000);
    // pros::delay(2000);

    // //ladder
    // chassis.turnToHeading(259.6, 1000);
    // chassis.moveToPoint(-20, -50.5, 2000);
    // chassis.waitUntilDone();
    // intake.move_voltage(0);
}

void redMogo() {
    // mogo
    chassis.moveToPoint(0, -30, 2000, {.forwards = false});
    chassis.waitUntilDone();
    mogoClamp.set_value(true);
    pros::delay(250);
    intake.move_voltage(-12000);
    pros::delay(250);

    // ring
    chassis.turnToHeading(-100, 1000);
    chassis.moveToPoint(-17.7, -35, 1000);
    pros::delay(250);

    // //ladder
    // chassis.turnToHeading(-236.4, 1000);
    // chassis.moveToPoint(-0.2, -45.8, 1000);
    // intake.move_voltage(0);

    // elims
    chassis.turnToHeading(-233.7, 2000);
    chassis.moveToPoint(-37, -19.6, 2000, {.forwards = false});
    chassis.waitUntilDone();
    mogoClamp.set_value(false);
    intake.move_voltage(0);
    chassis.moveToPoint(-24.5, -33, 2000);
    chassis.turnToHeading(7, 2000);
}

void blueMogo() {
    // mogo
    chassis.moveToPoint(0, -30, 2000, {.forwards = false});
    chassis.waitUntilDone();
    mogoClamp.set_value(true);
    pros::delay(250);
    intake.move_voltage(-12000);
    pros::delay(500);

    // ring
    chassis.turnToHeading(100, 1000);
    chassis.moveToPoint(17.7, -35, 1000);
    pros::delay(2000);

    // ladder
    chassis.turnToHeading(236.4, 1000);
    chassis.waitUntilDone();
    intake.move_voltage(0);
    mogoClamp.set_value(false);
    // chassis.moveToPoint(-0.8, -45.8, 1000);
    chassis.moveToPoint(-5.4, -44.6, 1000, {.maxSpeed = 50}); // 238.7
}

void autonomous() {
    chassis.calibrate(); // TODO: NEVER COMMENT OUT CALIBRATE OR SETPOSE OR ELSE IT WILL BREAK!!!!!!!!
    chassis.setPose(0, 0, 0);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    // initDebug();

    // chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);

    // chassis.turnToHeading(90, 100000);
    // pros::delay(1000);
    chassis.turnToHeading(180, 5000);
    // pros::delay(1000);
}

void opcontrol() {
    // chassis.calibrate(); //TODO: NEVER COMMENT OUT CALIBRATE OR SETPOSE OR ELSE IT WILL BREAK!!!!!!!!
    // chassis.setPose(0, 0, 0);

    // initDebug();

    // chassis.follow(interruptAutonomous_txt, interruptExtra_txt, 10, 1000000, true, false);

    chassis.calibrate();
    chassis.setPose(0, 0, 0);

    initO();

    int count = 1;
    int segCount = 1;

    while (true) {
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        chassis.arcade(leftY, rightX); // 0.9

        updateIntake();
        updateColorSort();
        updateClamp();
        updateDoink();
        updateLB();
        runLB();

        if (count == 10) { // TODO: DATA WRITTEN EVERY 0.1 SECONDS
            writePose();
            writeAdditional();
            count = 1;
            fileO.flush();
            fileOTwo.flush();
        }

        // pros::lcd::print(4, std::to_string(round(leftMotors.get_voltage()) * 1000.0 / 1000.0).c_str(), 0);
        // pros::lcd::print(5, std::to_string(round(rightMotors.get_voltage()) * 1000.0 / 1000.0).c_str(), 0);
        // pros::lcd::print(6, std::to_string(round(leftMotors.get_voltage() + rightMotors.get_voltage()) * 1000.0 /
        // 1000.0 / 2).c_str(), 0);

        // if(segCount == 100) { //TODO: NEW SEGMENT EVERY 5 SECONDS
        //     section++;
        //     segCount = 1;
        // }

        count++;
        // segCount++;

        closeO();

        pros::delay(10);
    }
}