
#include "main.h"
#include "ports.hpp"
#include "intakePiston.hpp"
#include "globals.hpp"

void intakeClampInit() { intakeClamp.set_value(false); }

int intakeClampState = 0;

void updateIntakeClamp() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
        if (intakeClampState == 0) {
            intakeClamp.set_value(true);
            intakeClampState++;
        } else if (intakeClampState == 2) {
            intakeClamp.set_value(false);
            intakeClampState++;
        }
    } else if (!controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
        if (intakeClampState == 1) {
            intakeClampState++;
        } else if (intakeClampState == 3) {
            intakeClampState = 0;
        }
    }
}