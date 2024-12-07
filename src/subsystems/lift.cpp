#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ports.hpp"
#include "lift.hpp"
#include "globals.hpp"

void liftInit() { lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); }

void updateLift() {
    lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { // lift up/down
        lift.move_velocity(-100);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { // lift up/down
        lift.move_velocity(100);
    } else {
        lift.move_velocity(0);
    }

    /*
       if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { //lift up/down
            lift.move_velocity(-100);
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { //lift up/down
            lift.move_velocity(100);
        } //else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            //lift.move_absolute(143,40);
        else {lift.move_velocity(0);}*/

    //lb macros
    int leftButton = 0;
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        if (leftButton == 0) {
            while(rotationSensor.get_position()!=25.83)
                lift.move_voltage(12000);
            lift.move_voltage(0);
            //lift.move_absolute(25.83, 100);
            leftButton++;
        } else if (leftButton == 2) {
            while(rotationSensor.get_position()!=150)
                lift.move_voltage(12000);
            lift.move_voltage(0);
            //lift.move_absolute(150, 100);
            leftButton++;
        } else if (leftButton == 4) {
            while(rotationSensor.get_position()!=0)
                lift.move_voltage(-12000);
            lift.move_voltage(0);
            //lift.move_absolute(0, 100);
            leftButton++;
        }
    } else if (!controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        if (leftButton == 1 || leftButton == 3) {
            leftButton++;
        } else if (leftButton == 5) {
            leftButton = 0;
        }
    }
}
