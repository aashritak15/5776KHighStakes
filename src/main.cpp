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


    //auton selector 

    
    

   
    
    pros::lcd::initialize(); // initialize brain screen
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    clampInit();
    intakeInnit();
    // liftInit();
    // intakeClampInit();
    // opticalInit();
    // initO();

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
            // pros::lcd::print(3, "Rotation (Lift): %i", rotationSensor.get_position()); // lift encoder
            // pros::lcd::print(4, "Intake State: %f", intakeState);

            std::vector<double> left = leftMotors.get_position_all();
            std::vector<double> right = rightMotors.get_position_all();
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
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    chassis.calibrate(); //TODO: NEVER COMMENT OUT CALIBRATE OR SETPOSE OR ELSE IT WILL BREAK!!!!!!!!
    chassis.setPose(0, 0, 0);

    initDebug();

    chassis.follow(autonomous_txt, extra_txt, 10, 40000, true, false);
}

// Lady Brown PID Functions 
double liftTarget;
float liftOutput;

void setLiftTarget(double target) { liftTarget = target; }
lemlib:: PID liftPID( 3, 10, 0, 0);

/**
 * Runs in driver control
 */

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
        updateClamp();
        updateLB();
        //updateLadyPID();
        //updateLadyTask();
        // updateLB();

        if(count == 10) {
            writePose();
            writeAdditional();
            count = 1;
            fileO.flush();
            fileOTwo.flush();
        }

        if(segCount == 100) {
            section++;
        }

        count++;
        segCount++;

        closeO();

        pros::delay(10);
    }
}