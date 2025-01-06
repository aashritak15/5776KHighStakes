#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ports.hpp"
#include "ladybrown.hpp"
#include "globals.hpp"

pros::Rotation rotationSensor(13);

void liftInit() { lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); }

const int position1 = 0;
const int position2 = 0;
const int position3 = 0;

double kp = 0;
double kd = 0;

int target = position1;

double prevError = 0;

bool pidControl = false;

bool ladyState = 0;

void updateLadyPID() {
    if (pidControl) {
        int current = rotationSensor.get_position();

        double error = target - current;

        double derivative = error - prevError;

        prevError = error;

        double power = ((kp * error) + (kd * derivative)) * 100;

        lift.move_voltage(power);
    }

    else {
        lift.move_voltage(0);
    }
}

void updateLadyTask() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        target = position1;
        pidControl = true;
    }

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        if (target == position1) {
            target = position2;
            ladyState = true;
        }

        else if (ladyState) {
            target = position3;
            ladyState = false;
        }

        else {
            target = position2;
            ladyState = true;
        }

        pidControl = true;
    }
}
