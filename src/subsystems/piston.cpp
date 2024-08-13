
#include "main.h"
#include "subsystems/ports.hpp"
#include "subsystems/pistons.hpp"
#include "globals.hpp"

pros::adi::DigitalOut mogoClamp('A');

void clampInit() { mogoClamp.set_value(false); }

int clampState = 0;

void updateClamp() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
        if (clampState == 0) {
            mogoClamp.set_value(true);
            clampState++;
        } else if (clampState == 2) {
            mogoClamp.set_value(false);
            clampState++;
        }
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
        if (clampState == 1) {
            clampState++;
        } else if (clampState == 3) {
            clampState = 0;
        }
    }
}