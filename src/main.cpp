#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "ports.hpp"
#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "ladybrown.hpp"
#include "intakeFirst.hpp"
#include "intakePiston.hpp"
#include "autons.hpp"
#include "magic.hpp"
#include <iostream>
#include <cmath>

// #include "autoSelect/selection.h"

// rd::Selector selector({
//    {"Blue AWP", &auton0},
//    {"Red AWP", &simple_auton},
//    {"Blue Rush" , &auton1},
//    {"Red Rush", &auton2}
//    {"Skills Run", &skills}
// });

/* TODO: PORT REVEAL!
    second stage: 14
    lb: 17
*/

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors
    chassis.setPose(0, 0, 0); //TODO: see if this fixed it

    clampInit();
    intakeInnit();
    liftInit();
    intakeClampInit();
    opticalInit();
    rotationInit();
    // initO(); TODO: in main so commented out

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

            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            pros::lcd::print(3, "Rotation (Lift): %i", rotationSensor.get_position()); // lift encoder
            pros::lcd::print(4, "Intake State: %f", intakeState);
            // pros::lcd::print(5, "Vert: %i", vertical.get_position());
            // pros::lcd::print(6, "Horiz: %i", horizontal.get_position());
            //pros::lcd::print(7 "Color: %f", optical.get_hue());

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

ASSET(autonomous_txt); //TODO: add std functionality
ASSET(extra_txt);


void autonomous() {
    initDebug();
    chassis.setPose(0, 0, 0, true);
    chassis.follow(autonomous_txt, extra_txt, 10, 40000, true, false); 
}

/**
 * Runs in driver control
 */

void opcontrol() {
    initO();
    chassis.calibrate(); // calibrate sensors
    chassis.setPose(0, 0, 0, true);

    while(true) {
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        chassis.arcade(leftY, rightX);

        updateIntake();
        updateClamp();

        // lemlib::Pose pose = chassis.getPose();
        // std::int32_t left = leftMotors.get_voltage();
        // std::int32_t right = rightMotors.get_voltage();  

        // std::cout<<std::to_string(left) + "\n";
        // std::cout<<std::to_string(right) + "\n";

        static unsigned long lastWriteTime = 0; // Tracks the last time writePose was called
        unsigned long currentTime = pros::millis(); // Get the current time in milliseconds

        if (currentTime - lastWriteTime >= 100) { //TODO: tune this value
            writePose();
            writeAdditional();
            lastWriteTime = currentTime; // Update the last write time
        }

        closeO();

        pros::delay(10);
    }


    
    //     updateIntakeFirst();
    //     updateClamp();
    //     updateIntakeClamp();
    //     updateLift();
    //     updateColorToggle();
    //     colorSort();

    //     if (sortState == 0) {
    //         controller.set_text(0, 0, "no sort   ");
    //     } else if (sortState == 1) {
    //         controller.set_text(0, 0, "scores blue");
    //     } else if (sortState == 2) {
    //         controller.set_text(0, 0, "scores red ");
    //     }

    //     if (sortState == 0) {
    //         controller.set_text(0, 0, "no sort   ");
    //     } else if (sortState == 1) {
    //         controller.set_text(0, 0, "scores blue");
    //     } else if (sortState == 2) {
    //         controller.set_text(0, 0, "scores red ");
    //     }
    
}
