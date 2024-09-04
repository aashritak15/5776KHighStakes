#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ports.hpp"
#include "intake.hpp"
#include "globals.hpp"

void intakeInnit() { intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST); }

int intaking = 0;

void updateIntake() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        if (intaking == 0) {
            intake.move_voltage(-12000);
            intaking++;
        } else if (intaking == 2) {
            intake.move_voltage(0);
            intaking++;
        }
    } else if (!controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        if (intaking == 1) {
            intaking++;
        } else if (intaking == 3) {
            intaking = 0;
        }
    }

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
        if (intaking == 0) {
            intake.move_voltage(12000);
            intaking++;
        } else if (intaking == 2) {
            intake.move_voltage(0);
            intaking++;
        }
    } else if (!controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
        if (intaking == 1) {
            intaking++;
        } else if (intaking == 3) {
            intaking = 0;
        }
    }
}
