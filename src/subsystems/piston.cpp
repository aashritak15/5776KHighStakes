
#include "main.h"
#include "subsystems/ports.hpp"
#include "subsystems/pistons.hpp"
#include "globals.hpp"

using namespace okapi;

pros::ADIDigitalOut mogoClamp = pros::ADIDigitalOut(clampPort);

void clampInit() { mogoClamp.set_value(false); }

int clampState = 0;

void updateClamp() {
    if (controller.getDigital(ControllerDigital::A) == 1) {
        if (clampState == 0) {
            mogoClamp.set_value(true);
            clampState++;
        } else if (clampState == 2) {
            mogoClamp.set_value(false);
            clampState++;
        }
    } else if (controller.getDigital(ControllerDigital::A) == 0) {
        if (clampState == 1) {
            clampState++;
        } else if (clampState == 3) {
            clampState = 0;
        }
    }
}