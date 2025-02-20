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

    //pros::Task pd_task1(lbTask, "lb task");
}

double globalTarget = 0;

void updateLB() {
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        ladyBrown.move_voltage(12000);
    } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        ladyBrown.move_voltage(-12000);
    } else {
        ladyBrown.move_voltage(0);
    }
    
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) { //*ZERO
        globalTarget = 3;

    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { //*LOAD
        globalTarget = 40;

    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { //*FULLSCORE
        globalTarget = 180;

    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) { //*STRAIGHT UP
        globalTarget = 120;

    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) { //*SCORE MOGO
        globalTarget = 60;
    }
}

void lbTask() {
    double currentAngle;
    double prevSpeedError = 0;
    double speedError = 0;
    double derivative;
    double armMoveSpeed;

    double kP = 0.9;
    double kD = 0.9; // 0.86

    while (true) {
        currentAngle = lbRotation.get_position() / 100.0;

        speedError = globalTarget - currentAngle;

        derivative = speedError - prevSpeedError;

        armMoveSpeed = (kP * speedError) + (kD * derivative);

        if (std::abs(armMoveSpeed) > 70) { armMoveSpeed = (armMoveSpeed < 0) ? -70 : 70; }
        if (std::abs(armMoveSpeed) < 3) { armMoveSpeed = 0; }

        armMoveSpeed = (kP * speedError) + (kD * derivative);
        ladyBrown.move_velocity(armMoveSpeed);

        prevSpeedError = speedError;

        pros::Task::delay(10);
    }
}