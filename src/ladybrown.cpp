#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ladybrown.hpp"
#include "globals.hpp"
#include "lemlib/timer.hpp"
#include <cmath>

void ladyBrownInit() { 
    ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); 
    ladyBrown.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    lbRotation.reset_position();
    lbRotation.set_position(0);
}

double target = 0;
//double ladyTarget = 0;
double prevDistance = 0;
double prevDistance1 = 0;
double kP = 0.65;
double kD = 0.65;
//bool start = false;

void updateLB() {
    //std::cout<<start<<" ";
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        target = 166;
        ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        //start = true;

        // ladyBrown.move_velocity(-100);

    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        
        target = 43.4;
        ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        target = 5;

        //start = true;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        target = 141.7;
        ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    } 
}

void runLB() {
    double currentAngle = lbRotation.get_position() / 100.0;
    double distance = target - currentAngle;
    double derivative = distance - prevDistance;
    double armMoveSpeed = (kP * distance) + (kD * derivative);
    // if (armMoveSpeed < 1) {
    //     armMoveSpeed = 0;
    // }
    ladyBrown.move_velocity(armMoveSpeed);
    prevDistance = distance;
}

void autonLB(double ladyTarget, int timeout) {
    lemlib::Timer timer(timeout);
    
    while(!timer.isDone() && abs(ladyTarget-lbRotation.get_position() / 100.0)>=1) {
        double currentAngle = lbRotation.get_position() / 100.0;
        double distance = ladyTarget - currentAngle;
        double derivative = distance - prevDistance1;
        double armMoveSpeed = (kP * distance) + (kD * derivative);
        // if (armMoveSpeed < 1) {
        //     armMoveSpeed = 0;
        // }
        ladyBrown.move_velocity(armMoveSpeed);
        prevDistance1 = distance;
    }
    ladyBrown.move_velocity(0);
}