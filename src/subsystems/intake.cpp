#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ports.hpp"
#include "intake.hpp"
#include "globals.hpp"

void intakeInnit() { intake.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); }

int intakeState = 0;

void updateIntake() {
    static bool buttonl1Pressed = false;
    static bool buttonxPressed = false;

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
