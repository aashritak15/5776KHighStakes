#include "piston.hpp"

void clampInit() { 
    mogoClamp.set_value(false); 
    doinkRight.set_value(false);
    doinkLeft.set_value(false);
    intakePiston.set_value(false);

    pros::Task pistonTask(runPistons, "pistons");
}

int clampState = 0;
int doinkRightState = 0;
int doinkLeftState = 0;
int intakePistonState = 0;

bool buttonYPressed = false;
bool buttonBPressed = false;
bool buttonXPressed = false;
bool buttonLeftPressed = false;

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

void updateDoinkRight() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
        if (!buttonBPressed) {
            buttonBPressed = true;
            if(doinkRightState == 0) {
                doinkRightState = 1;
            } else {
                doinkRightState = 0;
            }
        }
    } else {
        buttonBPressed = false;
    }
}

void updateDoinkLeft() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) { //TODO: change back to x
        if (!buttonXPressed) {
            buttonXPressed = true;
            if(doinkLeftState == 0) {
                doinkLeftState = 1;
            } else {
                doinkLeftState = 0;
            }
        }
    } else {
        buttonXPressed = false;
    }
}

void updateIntakePiston() {
    if (false) { //was up
        if (!buttonLeftPressed) {
            buttonLeftPressed = true;
            if(intakePistonState == 0) {
                intakePistonState = 1;
            } else {
                intakePistonState = 0;
            }
        }
    } else {
        buttonLeftPressed = false;
    }
}

void runPistons() {
    while(true) {
        if (clampState == 1) {
            mogoClamp.set_value(true);
        } else {
            mogoClamp.set_value(false);
        }

        if(doinkRightState == 1) {
            doinkRight.set_value(true);
        } else {
            doinkRight.set_value(false);
        }

        if(doinkLeftState == 1) {
            doinkLeft.set_value(true);
        } else {
            doinkLeft.set_value(false);
        }

        if(intakePistonState == 1) {
            intakePiston.set_value(true);
        } else {
            intakePiston.set_value(false);
        }
    }

    pros::delay(10);
}