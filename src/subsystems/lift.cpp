#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ports.hpp"
#include "lift.hpp"
#include "globals.hpp"

void liftInit() { lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); }

void updateLift() {
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        lift.move_velocity(-100);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        lift.move_velocity(100);
    } else {
        lift.move_velocity(0);
    }
}