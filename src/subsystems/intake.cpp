#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ports.hpp"
#include "intake.hpp"
#include "intakeFirst.hpp"
#include "globals.hpp"

void intakeInnit() { intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST); }

void opticalInit() { optical.set_led_pwm(75); }

int intakeState = 0;

int sortState = 0;

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

void sortTest() {
    static bool colorDetected = false;

    if(optical.get_hue() < 18 && optical.get_hue() > 12) { //TUNE
        if(!colorDetected) {
            colorDetected = true;
            intake.move_voltage(0);
            pros::Task::delay(200);
            intake.move_voltage(-12000);
            
            // intakeState = 3;
            // pros::delay(200);
            // intake.move_voltage(0);
            // pros::delay(100);
            // intake.move_voltage(-12000);
            // intakeState = 1;
        }
    } else {
        colorDetected = false;
    }
}

void updateColorToggle() {
    static bool leftPressed = false;
    static bool rightPressed = false;

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
        if(!leftPressed) {
            leftPressed = true;
            if(sortState == 0 || sortState == 2) {
                sortState == 1;
            } else if (sortState == 1) {
                sortState == 0;
            }
        }
    } else {
        leftPressed = false;
    }

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        if(!rightPressed) {
            rightPressed = true;
            if(sortState == 0 || sortState == 1) {
                sortState == 2;
            } else if (sortState == 2) {
                sortState == 0;
            }
        }
    } else {
        rightPressed = false;
    }
}

void colorSort() {
    static bool colorDetected = false;

    if(sortState = 1) {
        if(optical.get_hue() < 18 && optical.get_hue() > 12) { //TUNE
            if(!colorDetected) {
                colorDetected = true;
                intakeState = 3;
                intake.move_voltage(0);
                pros::Task::delay(200);
                intake.move_voltage(-12000);
                intakeState = 1;
            }
        } else {
            colorDetected = false;
        }
    } else if(sortState = 2) {
        if(optical.get_hue() < 216 && optical.get_hue() > 210) { //TUNE
            if(!colorDetected) {
                colorDetected = true;
                intakeState = 3;
                intake.move_voltage(0);
                pros::Task::delay(200);
                intake.move_voltage(-12000);
                intakeState = 1;
            }
        } else {
            colorDetected = false;
        }
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
