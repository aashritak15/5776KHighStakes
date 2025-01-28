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
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors
    chassis.setPose(0, 0, 0, false);

    // clampInit();
    // intakeInnit();
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
            pros::lcd::print(3, "Rotation (Lift): %i", rotationSensor.get_position()); // lift encoder
            pros::lcd::print(4, "Intake State: %f", intakeState);
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

    // chassis.turnToHeading(90, 1000);
    initDebug();
    mogoClamp.set_value(false); 

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
    //addSegment();

    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    // rerunPIDs();

    initO();

    // fileO<<"I LOVE T TEAM";
    // fileOTwo<<"I LOVE T TEAM";
    // if(!fileO || !fileOTwo) {
    //     pros::lcd::print(0, "i hate t team");
    // }
    // fileO.flush();
    // fileOTwo.flush();

    chassis.calibrate(); // calibrate sensors
    chassis.setPose(0, 0, 0);

    int count = 1;

    while (true) {
      
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        chassis.arcade(leftY, rightX * 0.9);

        // updateIntake();
        // updateClamp();
        //updateLadyPID();
        //updateLadyTask();
        // updateLB();


        // if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        //     setLiftTarget(24);
        //     liftOutput = liftPID.update(liftTarget - lift.get_position());
        //     lift.move(liftOutput);
        // }

        if(count == 10) {
            writePose();
            writeAdditional();
            count = 1;
            fileO.flush();
            fileOTwo.flush();
        }

        count++;

        closeO();

        pros::delay(10);
    }
}