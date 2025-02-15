#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "ladybrown.hpp"
#include "magic.hpp"
#include "pros/rtos.h"
#include <cmath>
//#include "autoSelect/selection.h"


void initialize() {
    pros::lcd::initialize();
    if(pros::lcd::is_initialized()) {std::cout<<"yippeee\n";}

    clampInit();
    doinkInit();
    intakeInit();
    ladyBrownInit();
    //selector::init();

    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE); //TODO: need to change back to coast
    chassis.calibrate(true);
    chassis.setPose(0, 0, 0);

    pros::Task callScreenTask(screenTask, "screen task");

}

// Runs while the robot is disabled
void disabled() {}

// Runs after initialize if the robot is connected to field control
void competition_initialize() {}

ASSET(autonomous_txt); // TODO: add std functionality
ASSET(extra_txt);

void autonomous() {
    // chassis.calibrate();
    // chassis.setPose(0, 0, 0);
    // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    // TODO: COMMENTED OUT BC TESTING IN INITIALIZE
    initDebug();

    //chassis.follow(autonomous_txt, extra_txt, "adsf");

    // if(color == 0) { //red
    //     sortState = 2;
    //     switch(auton) {
    //         case 1: //solo wp
    //             chassis.follow(autonomous_txt, extra_txt, "adsf");
    //             break;
    //         case 2: //mogo rush
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 3:
    //             chassis.moveToPoint(0, 24, 10000);
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 4:
    //             chassis.turnToHeading(90, 10000);
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 5:
    //             chassis.turnToHeading(180, 10000);
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 6:
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 7:
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 8:
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //     }
    // } else if(color == 1) { //blue
    //     sortState = 1;
    //     switch(auton) {
    //         case 1: //solo wp
    //             chassis.follow(autonomous_txt, extra_txt, "adsf");
    //             break;
    //         case 2: //mogo rush
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 3:
    //             chassis.moveToPoint(0, 24, 10000);
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 4:
    //             chassis.turnToHeading(90, 10000);
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 5:
    //             chassis.turnToHeading(180, 10000);
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 6:
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 7:
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 8:
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //     }
    // }

    // chassis.turnToHeading(90, 100000);
    // chassis.turnToHeading(180, 5000);
}

void opcontrol() {
//     chassis.follow(autonomous_txt, extra_txt, "red");

    initO();

    int count = 1;

    while (true) {
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        chassis.arcade(leftY, rightX); // 0.9

        updateIntake();
        updateColorSort();
        updateClamp();
        updateDoink();
        updateLB();
        // lbTask();

        if (count == 5) { //*data written every 0.1 seconds
            writePose();
            writeAdditional();
            count = 1;
            fileO.flush();
            fileOTwo.flush();

            // writeInterruptPose(); //*INTERRUPT
            // writeInterruptAdditional();
            // count = 1;
            // fileInterrupt.flush();
            // fileInterruptTwo.flush();
        }
        count++;

        closeO();
        // closeOInterrupt(); //*INTERRUPT

        pros::delay(10);
    }

}