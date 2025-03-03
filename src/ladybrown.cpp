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
bool comingDown = false;

void updateLB() { //TODO: outdated angles
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) { //*ZERO
        globalTarget = 0;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { //*LOAD
        globalTarget = 25;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { //*FULLSCORE
        globalTarget = 140;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) { //*STRAIGHT UP
        globalTarget = 100;
    }
}

void updateLBTask() {
    int prevIntakeState;

    while (true) {
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) { //*ZERO
            globalTarget = 0;
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { //*LOAD
            globalTarget = 26;

            // if(comingDown) { //diff angle for coming down because i think pid less accurate when going from 0 to load
            //     globalTarget = 16;
            //     pros::delay(200);
            //     comingDown = false;
            // } else {
            //     globalTarget = 21.5;
            // }
            //globalTarget = 21.5;
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { //*FULLSCORE
            prevIntakeState = intakeState;
            intakeState = 2;
            pros::delay(25);
            globalTarget = 140;
            intakeState = prevIntakeState;

            // comingDown = true;
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) { //*STRAIGHT UP
            prevIntakeState = intakeState;
            intakeState = 2;
            pros::delay(25);
            globalTarget = 102.32;
            intakeState = prevIntakeState;

            // comingDown = true;
        }
        //std::cout<<"lb: "<<lbRotation.get_position() / 100.0<<"\n";//<<", comingDown: "<<comingDown<<"\n";
        pros::delay(10);
    }
}

void lbTask() {
    double currentAngle;
    double prevError = 0;
    double error = 0;
    double derivative;
    double armMoveVoltage;

    double kP = 0.3;
    double kD = 0.6; // 0.86

    while (true) {        
        currentAngle = lbRotation.get_position() / 100.0;

        error = globalTarget - currentAngle;

        derivative = error - prevError;

        armMoveVoltage = (kP * error) + (kD * derivative);

        // Scale the output from -100 to 100 range to -12000 to 12000
        armMoveVoltage = armMoveVoltage * 1200;

        if (std::abs(armMoveVoltage) > 12000) { armMoveVoltage = (armMoveVoltage < 0) ? -12000 : 12000; }
        if (std::abs(armMoveVoltage) < 200) { armMoveVoltage = 0; } // Adjusted small deadzone for voltage

        ladyBrown.move_voltage(-armMoveVoltage);

        prevError = error;

        pros::Task::delay(10);
    }
}