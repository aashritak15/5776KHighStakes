#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ports.hpp"
#include "intakeFirst.hpp"
#include "globals.hpp"

void intakeFirstInnit() { intakeFirst.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); }

int intakingFirst = 0;

void updateIntakeFirst() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        if (intakingFirst == 0) {
            intakeFirst.move_velocity(-600);
            intakingFirst++;
        } else if (intakingFirst == 2) {
            intakeFirst.move_velocity(0);
            intakingFirst++;
        }
    } else if (!controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        if (intakingFirst == 1) {
            intakingFirst++;
        } else if (intakingFirst == 3) {
            intakingFirst = 0;
        }
    }
}
