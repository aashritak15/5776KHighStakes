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
                pros::lcd::print(6, "Score red");
            } else if (sortState == 2) {
                sortState = 0;
                controller.set_text(0, 0, "no sort         ");
            }
        }
    } else {
        buttonUpPressed = false;
    }
}

/**
 *@brief run color sort based on global state
 */
void runColorSort() {
    while (true) {
        if (intakeState == 0) { // TODO: see if this fixes
            pros::delay(10);
            continue;
        }

        if (sortState == 1) {
            if (optical.get_hue() < 25 && optical.get_hue() > 10 && optical.get_proximity() > 200) {
                if (!colorDetected) {
                    colorDetected = true;
                    pros::Task::delay(40);
                    intakeState = 2;
                    pros::Task::delay(300);
                    intakeState = 1;

                } else {
                    colorDetected = false;
                }
            }
        } else if (sortState == 2) {
            if (optical.get_hue() < 230 && optical.get_hue() > 210 && optical.get_proximity() > 200) {
                if (!colorDetected) {
                    colorDetected = true;
                    pros::Task::delay(40);
                    intakeState = 2;
                    pros::Task::delay(300);
                    intakeState = 1;
                }
            } else {
                colorDetected = false;
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

// void antiJam() {
//     while (true) {
//         // Detect a jam based on velocity and current draw
//         if (intakeUpper.get_actual_velocity() < 100 && intakeLower.get_current_draw() > 2400) {
//             intakeUpper.move_voltage(12000); // Try to push forward for a moment
//             pros::Task::delay(40);
//             intakeUpper.move_voltage(-12000); // Reverse to clear jam
//             pros::Task::delay(200); // Hold reverse for longer in case of severe jam
//             intakeUpper.move_voltage(6000); // Resume with reduced power before full power
//             pros::Task::delay(100);
//             intakeUpper.move_voltage(12000);
//         }

//         //         pros::delay(10);
//     }
// }