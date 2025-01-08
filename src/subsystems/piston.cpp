
#include "main.h"
#include "ports.hpp"
#include "piston.hpp"
#include "globals.hpp"

void clampInit() { mogoClamp.set_value(false); }
void doinkInit() { doink.set_value(false); }


int clampState = 0;
int doinkState = 0;

void updateClamp() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
        if (clampState == 0) {
            mogoClamp.set_value(true);
            clampState++;
        } else if (clampState == 2) {
            mogoClamp.set_value(false);
            clampState++;
        }
    } else if (!controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
        if (clampState == 1) {
            clampState++;
        } else if (clampState == 3) {
            clampState = 0;
        }
    }
}

void updateDoink() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
        if (doinkState == 0) {
            doink.set_value(true);
            doinkState++;
        } else if (doinkState == 2) {
            doink.set_value(false);
            doinkState++;
        }
    } else if (!controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
        if (doinkState == 1) {
            doinkState++;
        } else if (doinkState == 3) {
            doinkState = 0;
        }
    }
}