
#include "main.h"
#include "ports.hpp"
#include "piston.hpp"
#include "globals.hpp"

void clampInit() { mogoClamp.set_value(false); }
void doinkInit() { doink.set_value(false); }


int clampState = 0;
int doinkState = 0;

void updateClamp() {
    static bool buttonYPressed = false;

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
        if (!buttonYPressed) {
            buttonYPressed = true;
            if(clampState == 0) {
                clampState = 1;
                mogoClamp.set_value(true);
            } else {
                clampState = 0;
                mogoClamp.set_value(false);
            }
        }
    } else {
        buttonYPressed = false;
    }
}

void updateDoink() { //TODO: FIX DOINK TO MATCH CLAMP
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