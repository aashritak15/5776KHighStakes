#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ladybrown.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "lemlib/timer.hpp"
#include <cmath>

void ladyBrownInit() {
    ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    ladyBrown.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    // lbRotation.reset_position();
    lbRotation.set_position(0);

    pros::Task pd_task1(lbTask, "lb run task");
    pros::Task updateLBTask2(updateLBTask, "lb update task");
}

double globalTarget = 0;

void updateLB() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) { //*ZERO
        globalTarget = 0;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { //*LOAD
        globalTarget = 21.5;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { //*FULLSCORE
        globalTarget = 140;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) { //*STRAIGHT UP
        globalTarget = 102.32;
    }
}

void updateLBTask() {
    int prevIntakeState;

    while (true) {
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) { //*ZERO
            globalTarget = 0;
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { //*LOAD
            globalTarget = 21.5;
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { //*FULLSCORE
            globalTarget = 140;
            prevIntakeState = intakeState;
            intakeState = 2;
            pros::delay(200);
            intakeState = prevIntakeState;
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) { //*STRAIGHT UP
            globalTarget = 102.32;
            prevIntakeState = intakeState;
            intakeState = 2;
            pros::delay(200);
            intakeState = prevIntakeState;
        }
        std::cout<<lbRotation.get_position() / 100.0<<"\n";
        pros::delay(10);
    }
}

void lbTask() {
    double currentAngle;
    double prevSpeedError = 0;
    double speedError = 0;
    double derivative;
    double armMoveVoltage;

    double kP = 0.1;
    double kD = 0; // 0.86

    while (true) {        
        currentAngle = lbRotation.get_position() / 100.0;

        speedError = globalTarget - currentAngle;

        derivative = speedError - prevSpeedError;

        armMoveVoltage = (kP * speedError) + (kD * derivative);

        // Scale the output from -100 to 100 range to -12000 to 12000
        armMoveVoltage = armMoveVoltage * 1200;

        if (std::abs(armMoveVoltage) > 12000) { armMoveVoltage = (armMoveVoltage < 0) ? -12000 : 12000; }
        if (std::abs(armMoveVoltage) < 360) { armMoveVoltage = 0; } // Adjusted small deadzone for voltage

        ladyBrown.move_voltage(-armMoveVoltage);

        prevSpeedError = speedError;

        pros::Task::delay(10);
    }
}