#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "intake.hpp"
#include "globals.hpp"
#include <iostream>

int intakeState = 0;
int sortState; // 1 = score blue sort red, 2 = score red sort blue
bool buttonUpPressed = false;
bool colorDetected = false;

// bool buttonUpPressed = false;
// bool colorDetected = false;

void intakeInit() {
    intakeUpper.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    intakeLower.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    optical.set_integration_time(10);
    optical.set_led_pwm(100);

    pros::Task sortTask(runColorSort, "color sort");
    pros::Task intakeTask(runIntake, "intake");
    // pros::Task antiJamTask(antiJam, "anti jam");
}

/**
 *@brief update intake global state based on button inputs
 */
void updateIntake() {
    static bool buttonl1Pressed = false;
    static bool buttonxPressed = false;

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        if (!buttonl1Pressed) {
            buttonl1Pressed = true;
            if (intakeState == 0 || intakeState == 2) {
                intakeState = 1;
            } else if (intakeState == 1) {
                intakeState = 0;
            }
        }
    } else {
        buttonl1Pressed = false;
    }

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        if (!buttonxPressed) {
            buttonxPressed = true;
            if (intakeState == 0 || intakeState == 1) {
                intakeState = 2;
            } else if (intakeState == 2) {
                intakeState = 0;
            }
        }
    } else {
        buttonxPressed = false;
    }
}

/**
 *@brief update color sort global state based on button inputs
 */
void updateColorSort() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
        if (!buttonUpPressed) {
            buttonUpPressed = true;
            if (sortState == 0) {
                sortState = 1;
                controller.set_text(0, 0, "scores blue      ");
            } else if (sortState == 1) {
                sortState = 2;
                controller.set_text(0, 0, "scores red       ");
            } else if (sortState == 2) {
                sortState = 0;
                controller.set_text(0, 0, "no sort         ");
            }
        }
    } else {
        buttonUpPressed = false;
    }
}

bool intakeKeep;
int timeCount = 0;

/**
 *@brief run color sort based on global state
 */
void runColorSort() {
    int prevIntakeKeep = 0;

    while (true) {
        // std::cout<<std::to_string(optical.get_hue())<<" ";
        // std::cout<<std::to_string(optical.get_proximity());
        // std::cout<<"\n";
        
        if (intakeKeep) {
            if(optical.get_hue() < 30 && optical.get_hue() > 0 && optical.get_proximity() > 200) {
                if(prevIntakeKeep != intakeKeep) {
                    intakeState = -1;
                    intakeUpper.move_velocity(-300);
                    intakeLower.move_velocity(100);
                    pros::delay(80);
                }

                intakeState = 0;
            }
        }

        prevIntakeKeep = intakeKeep;

            //velocity fluctuates too much to use
            // if(intakeState != 0 && intakeUpper.get_actual_velocity() < 300) {
            //     timeCount += 1;
            //     std::cout<<"timer: "<<timeCount<<"\n";
            //     if(timeCount >= 500) {
            //         pros::delay(500);
            //         intakeState = 0;
            //         timeCount = 0;
            //         pros::delay(10);
            //         continue;
            //     }

            //     pros::delay(10);
            //     continue;
            // } else {
            //     timeCount = 0;
            //     pros::delay(10);
            //     continue;
            // }
        

        if (intakeState == 0) { // TODO: see if this fixes
            pros::delay(10);
            continue;
        }

        if (sortState == 1) {
            if (optical.get_hue() < 30 && optical.get_hue() > 0 && optical.get_proximity() > 200) {

                pros::Task::delay(35);

                intakeState = -1; //TODO: special sauce intake state
                intakeUpper.move_voltage(-12000);
                intakeLower.move_voltage(12000);

                pros::Task::delay(200);

                intakeState = 1;
                std::cout<<"red detected\n";

            }
        } else if (sortState == 2) {
            if (optical.get_hue() < 230 && optical.get_hue() > 200 && optical.get_proximity() > 200) {

                pros::delay(35);

                intakeState = -1; //TODO: special sauce intake state
                intakeUpper.move_voltage(-12000);
                intakeLower.move_voltage(12000);

                pros::Task::delay(200);

                intakeState = 1;
                std::cout<<"blue detected\n";
            }
        }

        pros::delay(10);
    }

}


/**
 *@brief run intake motors based on global state
 */
void runIntake() {

    while (true) {
        if (intakeState == 0) {
            intakeUpper.move_voltage(0);
            intakeLower.move_voltage(0);
        } else if (intakeState == 1) {
            intakeUpper.move_voltage(12000);
            intakeLower.move_voltage(-12000);
        } else if (intakeState == 2) {
            intakeUpper.move_voltage(-12000);
            intakeLower.move_voltage(12000);
        }

        pros::delay(10);
    }
}

void antiJam() {

    int jamCount;

    while (true) {
        if (intakeUpper.get_actual_velocity() < 100 && intakeState == 1) { //TODO: ANTI JAM
            jamCount++;

            if (jamCount == 50) {
                intakeState = 2;
                pros::delay(250);
                intakeState = 1;
            }
        } else {
            jamCount = 0;
        }

        pros::delay(10);
    }
}