#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ports.hpp"
#include "intake.hpp"
#include "intakeFirst.hpp"
#include "globals.hpp"

void intakeInnit() { intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST); }

int intakeState = 0;

bool colorSort = false;

void updateIntake() {
    static bool buttonl1Pressed = false;
    static bool buttonxPressed = false;
   
    // bool buttonaPressed = false;

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        if (!buttonl1Pressed) {
            buttonl1Pressed = true;
            if (intakeState == 0 || intakeState == 2) {
                intake.move_voltage(-12000);
                intakeState = 1;
            } else if (intakeState == 1) {
                intake.move_voltage(0);
                intakeState = 0;
            }
        }
    } else {
        buttonl1Pressed = false;
    }

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
        if (!buttonxPressed) {
            buttonxPressed = true;
            if (intakeState == 0 || intakeState == 1) {
                intake.move_voltage(12000);
                intakeState = 2;
            } else if (intakeState == 2) {
                intake.move_voltage(0);
                intakeState = 0;
            }
        }
    } else {
        buttonxPressed = false;
    }
}

void updateColorSortRed() {
    static bool colorDetected = false;

    if(optical.get_hue() < 100 && optical.get_hue() > 100) { //TUNE
        if(!colorDetected){
            colorDetected = true;
            intakeState = 3;
            pros::delay(200); //TUNE
            intake.move_voltage(0);
            pros::delay(200); //TUNE
            intake.move_voltage(12000);
        }
    } else {
            colorDetected = false;
            intakeState = 0;
    }
}

// void resetIntake() {
//     // if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
//     //     //intake loop value:
//     //     int onethird;
//     //     if(intake.get_position()/onethird == 0) {
//     //         intake.move_absolute(0, 200);
//     //     } else if(intake.get_position()/onethird == 1) {
//     //         intake.move_absolute(onethird, 200);
//     //     } else if(intake.get_position()/onethird == 2) {
//     //         intake.move_absolute(2*onethird, 200);
//     //     }
//     // }

//     if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
//         if (intake.get_position() / 120 == 0) {
//             intake.move_absolute(0, 200);
//         } else if (intake.get_position() / 120 == 1) {
//             intake.move_absolute(120, 200);
//         } else if (intake.get_position() / 120 == 2) {
//             intake.move_absolute(240, 200);
//         }
//     }
// }

// void stepIntake() {
//     int onethird;
//     if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) { intake.move_absolute(onethird, 200); }
// }
