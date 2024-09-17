#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ports.hpp"
#include "intake.hpp"
#include "intakeFirst.hpp"
#include "globals.hpp"

void intakeInnit() { intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST); }

int intakeState = 0;

static bool buttonl1Pressed = false;
static bool buttonaPressed = false;

void updateIntake() {
    buttonl1Pressed = false;
    buttonaPressed = false;

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        if (!buttonl1Pressed) {
            buttonl1Pressed = true;
            if (intakeState == 0 || intakeState == 2) {
                intake.move_voltage(-12000); // Intake runs inward
                intakeState = 1;
            } else if (intakeState == 1) {
                intake.move_voltage(0); // Stop intake
                intakeState = 0;
            }
        }
    } else {
        buttonl1Pressed = false;
    }

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
        if (!buttonaPressed) {
            buttonaPressed = true;
            if (intakeState == 0 || intakeState == 1) {
                intake.move_voltage(12000); // Outtake
                intakeState = 2;
            } else if (intakeState == 2) {
                intake.move_voltage(0); // Stop intake
                intakeState = 0;
            }
        }
    } else {
        buttonaPressed = false;
    }
}

void resetIntake() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        if (intake.get_position() / 120 == 0) {
            intake.move_absolute(0, 200);
        } else if (intake.get_position() / 120 == 1) {
            intake.move_absolute(120, 200);
        } else if (intake.get_position() / 120 == 2) {
            intake.move_absolute(240, 200);
        }
    }
}

void stepIntake() {
    int onethird;
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) { intake.move_absolute(onethird, 200); }
}

void intakeFirstInnit() { intakeFirst.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); }

int intakingFirstState = 0;

void updateIntakeFirst() {
    static bool buttonl2Pressed = false;
    static bool buttonaPressed = false;

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        if (!buttonl2Pressed) {
            buttonl2Pressed = true;
            if (intakingFirstState == 0 || intakingFirstState == 2) {
                intakeFirst.move_velocity(-600); // Intake runs inward
                intakingFirstState = 1;
            } else if (intakingFirstState == 1) {
                intakeFirst.move_velocity(0); // Stop intake
                intakingFirstState = 0;
            }
        }
    } else {
        buttonl2Pressed = false;
    }

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
        if (!buttonaPressed) {
            buttonaPressed = true;
            if (intakingFirstState == 0 || intakingFirstState == 1) {
                intakeFirst.move_velocity(600); // Outtake
                intakingFirstState = 2;
            } else if (intakingFirstState == 2) {
                intakeFirst.move_velocity(0); // Stop intake
                intakingFirstState = 0;
            }
        }
    } else {
        buttonaPressed = false;
    }
}
