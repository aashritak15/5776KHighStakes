#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ladybrown.hpp"
#include "globals.hpp"
#include "lemlib/timer.hpp"
#include <cmath>

void ladyBrownInit() {
    ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    ladyBrown.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    lbRotation.reset_position();
    lbRotation.set_position(0);
}

double target = 0;
double prevDistance = 0;
double prevDistance1 = 0;

double prevSpeedError = 0;

double kP = 0.6;
double kD = 0.7;

// bool start = false;

void updateLB() {
    //*LOAD: 43
    //*ALLIANCE STAKE: 236.2
    //*NEUTRAL STAKE: 170
    //*PASSIVE: 125.3


    // std::cout<<start<<" ";
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) { //*ALLIANCE STAKE
        target = 236.2;

    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { //*LOAD
        target = 43;

    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) { //*DISABLED/ZERO
        target = 5;

        // start = true;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { //*PASSIVE
        target = 125.3;

    }
}

void runLB() {
    double currentAngle = lbRotation.get_position() / 100.0; // get values

    double speedError = (target - currentAngle) / target * 100.0; //convert units

    double derivative = speedError - prevSpeedError;
    double armMoveSpeed = (kP * speedError) + (kD * derivative);

    if(std::abs(armMoveSpeed) > 100.0) { // ratio
        armMoveSpeed = 100.0;
    }

    ladyBrown.move_velocity(armMoveSpeed);
    // std::cout<<std::to_string(armMoveSpeed)<<"\n";
    prevSpeedError = speedError;
}

void autonLB(double ladyTarget, int timeout) {
    lemlib::Timer timer(timeout);

    while (!timer.isDone() && abs(ladyTarget - lbRotation.get_position() / 100.0) >= 1) {
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