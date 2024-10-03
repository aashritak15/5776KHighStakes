#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ports.hpp"
#include "lift.hpp"
#include "globals.hpp"


void liftInit() { lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); }

bool toggle = false;
bool initialLeft = false; //previous state of the left button
//int leftButton = 0;

void updateLift() {
    bool currentLeft = controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT);

    if(currentLeft && !initialLeft){
        toggle = !toggle;
    }

    initialLeft = currentLeft;
    if(!toggle) { //lock
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { //lift up/down
            lift.move_velocity(-100);
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { //lift up/down
            lift.move_velocity(100);
        } else {lift.move_velocity(0);}
    } else { //toggle to height
        lift.move_absolute(143, 40);
    }


   /*if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { //lift up/down
        lift.move_velocity(-100);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { //lift up/down
        lift.move_velocity(100);
    } //else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
        //lift.move_absolute(143,40);
    else {lift.move_velocity(0);}


    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
        if (leftButton == 0) {
            lift.move_absolute(143,40);
            leftButton++;
        } else if (leftButton == 2) {
            lift.move_absolute(0,40);
            leftButton++;
        }
    } else if (!controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
        if (leftButton == 1) {
            leftButton++;
        } else if (leftButton == 3) {
            leftButton = 0;
        }
    }*/
}
