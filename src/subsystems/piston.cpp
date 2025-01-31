
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

void updateDoink() {
    static bool buttonDoinkPressed = false;

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
        if (!buttonDoinkPressed) {
            buttonDoinkPressed = true;
            if(doinkState == 0) {
                doinkState = 1;
                doink.set_value(true);
            } else {
                doinkState = 0;
                doink.set_value(false);
            }
        }
    } else {
        buttonDoinkPressed = false;
    }
}