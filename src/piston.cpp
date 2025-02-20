#include "piston.hpp"

void clampInit() { 
    mogoClamp.set_value(false); 
    doink.set_value(false);

    pros::Task pistonTask(runPistons, "pistons");
}

int clampState = 0;
int doinkState = 0;

bool buttonYPressed = false;
bool buttonDoinkPressed = false;

void updateClamp() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
        if (!buttonYPressed) {
            buttonYPressed = true;
            if(clampState == 0) {
                clampState = 1;
            } else {
                clampState = 0;
            }
        }
    } else {
        buttonYPressed = false;
    }
}

void updateDoink() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
        if (!buttonDoinkPressed) {
            buttonDoinkPressed = true;
            if(doinkState == 0) {
                doinkState = 1;
            } else {
                doinkState = 0;
            }
        }
    } else {
        buttonDoinkPressed = false;
    }
}

void runPistons() {
    while(true) {
        if (clampState == 1) {
            mogoClamp.set_value(true);
        } else {
            mogoClamp.set_value(false);
        }

        if(doinkState == 1) {
            doink.set_value(true);
        } else {
            doink.set_value(false);
        }
    }

    pros::delay(10);
}