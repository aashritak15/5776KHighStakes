#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ports.hpp"
#include "intake.hpp"
#include "intakeFirst.hpp"
#include "globals.hpp"

void intakeFirstInnit() { intakeFirst.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); }

int intakingFirstState = 0;

void updateIntakeFirst() {
    static bool buttonl2Pressed = false;
    static bool buttonaPressed = false;

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        if (!buttonl2Pressed) {
            buttonl2Pressed = true;
            if (intakingFirstState == 0 || intakingFirstState == 2) {
                intakeFirst.move_velocity(-600);
                intakingFirstState = 1;
            } else if (intakingFirstState == 1) {
                intakeFirst.move_velocity(0);
                intakingFirstState = 0;
            }
        }
    } else {
        buttonl2Pressed = false;
    }

    if (controller.get_digital(
            pros::E_CONTROLLER_DIGITAL_A)) { // TODO: why is this vel and not voltage and why is rpm 600
        if (!buttonaPressed) {
            buttonaPressed = true;
            if (intakingFirstState == 0 || intakingFirstState == 1) {
                intakeFirst.move_velocity(600);
                intakingFirstState = 2;
            } else if (intakingFirstState == 2) {
                intakeFirst.move_velocity(0);
                intakingFirstState = 0;
            }
        }
    } else {
        buttonaPressed = false;
    }
}