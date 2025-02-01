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

    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    pros::Task screenTask([&]() {
        while (true) {

            //* line 7 reserved for rerun states.
            
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            pros::lcd::print(3, "Color: %f", optical.get_hue());
            pros::lcd::print(4, "LB Rot Sensor: %i", lbRotation.get_position());

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

void autonomous() {
    chassis.calibrate(); //TODO: NEVER COMMENT OUT CALIBRATE OR SETPOSE OR ELSE IT WILL BREAK!!!!!!!!
    chassis.setPose(0, 0, 0);

    initDebug();

    chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
}

void opcontrol() {
    chassis.calibrate();
    chassis.setPose(0, 0, 0);

    initO();

    int count = 1;
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

        if(count == 10) { //TODO: DATA WRITTEN EVERY 0.1 SECONDS
            writePose();
            writeAdditional();
            count = 1;
            fileO.flush();
            fileOTwo.flush();
        }

        if(segCount == 500) { //TODO: NEW SEGMENT EVERY 5 SECONDS
            section++;
            segCount = 1;
        }

        count++;
        segCount++;

        closeO();

        pros::delay(10);
    }

    if(pros::E_CONTROLLER_DIGITAL_RIGHT) {
    
}
}