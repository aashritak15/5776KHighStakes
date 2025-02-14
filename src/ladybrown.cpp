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

    pros::Task pd_task1(lbTask);
}

double globalTarget = 0;
double kP = 0.9;
double kD = 0.7;

// bool start = false;

void updateLB() {
    //*LOAD: 43
    //*ALLIANCE STAKE: 236.2
    //*NEUTRAL STAKE: 170
    //*PASSIVE: 125.3


    // std::cout<<start<<" ";
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) { //*ALLIANCE STAKE
        globalTarget = 236.2;

    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { //*LOAD
        globalTarget = 41.5;

    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) { //*DISABLED/ZERO
        globalTarget = 3;

        // start = true;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { //*PASSIVE
        globalTarget = 125.3;

    }
}

// void runLB() {
//     double currentAngle = lbRotation.get_position() / 100.0;
//     double distance = globalTarget - currentAngle;
//     double derivative = distance - prevDistance;
//     double armMoveSpeed = (kP * distance) + (kD * derivative);

//     if(std::abs(armMoveSpeed) > 65) {
//         if(armMoveSpeed < 0) {
//             armMoveSpeed = -65;
//         } else {
//             armMoveSpeed = 65;
//         }
//     }

//     ladyBrown.move_velocity(armMoveSpeed);
//     prevDistance = distance;
// }

void lbTask() {
    double currentAngle;
    double prevSpeedError = 0;
    double speedError = 0;
    double derivative;
    double armMoveSpeed;

    float kP = 0;
    float kD = 0;

    while(true) {
        currentAngle = lbRotation.get_position() / 100.0;
        speedError = (currentAngle - globalTarget) / globalTarget * 100;
        derivative = prevSpeedError - speedError;

        armMoveSpeed = (kP * speedError) + (kD * derivative);

        if(std::abs(armMoveSpeed) > 70) {
            if(armMoveSpeed < 0) {
                armMoveSpeed = -70;
            } else {
                armMoveSpeed = 70;
            }
        }

        ladyBrown.move_velocity(armMoveSpeed);
        prevSpeedError = speedError;

        pros::delay(10);
    }
}

// void autonLB(double ladyTarget, int timeout) {
//     lemlib::Timer timer(timeout);

//     while (!timer.isDone() && std::abs(ladyTarget - lbRotation.get_position() / 100.0) >= 1) {
//         double currentAngle = lbRotation.get_position() / 100.0;
//         double distance = ladyTarget - currentAngle;
//         double derivative = distance - prevDistance1;
//         double armMoveSpeed = (kP * distance) + (kD * derivative);
//         // if (armMoveSpeed < 1) {
//         //     armMoveSpeed = 0;
//         // }
//         ladyBrown.move_velocity(armMoveSpeed);
//         prevDistance1 = distance;
//     }
//     ladyBrown.move_velocity(0);
// }