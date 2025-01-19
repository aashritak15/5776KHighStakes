#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ports.hpp"
#include "ladybrown.hpp"
#include "globals.hpp"
#include <cmath>

void ladyBrownInit() { 
    ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); 
    ladyBrown.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    lbRotation.reset_position();
    lbRotation.set_position(0);
}

double target = 0;
double prevDistance = 0;
double kP = 0.65;
double kD = 0.65;
//bool start = false;

void updateLB() {
    //std::cout<<start<<" ";
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        target = 166;
        // ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        //start = true;

        // ladyBrown.move_velocity(-100);

    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {

        target = 41.4;

        // ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        //start = true;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        //target = -10;
        // ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

        target = 5;

        //start = true;
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {

        target = 141.7;

        // ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        //start = true;
    } 

    double currentAngle = lbRotation.get_position() / 100.0;
    pros::lcd::print(5, "Current angle: %f", currentAngle);
    double distance = target - currentAngle;
    double derivative = distance - prevDistance;
    double armMoveSpeed = (kP * distance) + (kD * derivative);
    pros::lcd::print(6, "Move speed: %f", armMoveSpeed);
    // if (armMoveSpeed < 1) {
    //     armMoveSpeed = 0;
    // }
    ladyBrown.move_velocity(armMoveSpeed);
    prevDistance = distance;
}
// ladybrown pid

// void updateLB() {
//     if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
//         setLiftTarget(24);
//         // pid that runs constantly
//         float liftOutput = liftPID.update(liftTarget - ladyBrown.get_position());
//         ladyBrown.move(liftOutput);
//     }

//     const int position1 = 24;
//     const int position2 = 100; // Adjust as per your desired positions
//     const int position3 = 0;

//     double kp = 0.0; // Set appropriate PID tuning constants
//     double kd = 0.0;

//     int target = position1;

//     double prevError = 0;

//     bool pidControl = false;

//     bool ladyState = false;

//     // PID Update Function
//     void updateLadyPID() {
//         if (pidControl) {
//             int current = rotationSensor.get_angle();

//             double error = target - current;
//             double derivative = error - prevError;

//             prevError = error;

//             double power = ((kp * error) + (kd * derivative)) * 100;

//             ladyBrown.move_voltage(power); // Assuming move_voltage accepts values in millivolts
//         } else {
//             ladyBrown.move_voltage(0);
//         }
//     }

//     // Task Update Function
//     void updateLadyTask() {
//         if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
//             target = position1; // Reset to position1
//             pidControl = true;
//             ladyState = false; // Reset state
//         }

//         if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
//             if (target == position1) {
//                 target = position2;
//             } else if (target == position2) {
//                 target = position3;
//             } else {
//                 target = position1; // Wrap back to position1
//             }
//             pidControl = true; // Enable PID control
//         }
//     }
// }

/// ---------------> Basic

// void updateLB() {
//     ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

//     if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { // lift up/down
//         ladyBrown.move_absolute(-700, -100);
//         ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
//     } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { // lift up/down
//         ladyBrown.move_absolute(-130, -100);
//         ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
//     } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
//         ladyBrown.move_absolute(-15, 100);
//         ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
//     } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
//         ladyBrown.move_absolute(-600, -100);
//         ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
//     } else {
//         ladyBrown.move_velocity(0);
//     }
// }

// Down is go to -145 but coast
// right is -600 brake

// void updateLB() {
//     ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

//     if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { // lift up/down
//         ladyBrown.move_absolute(390, -200);
//         ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
//         // ladyBrown.move_velocity(-100);
//     } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { // lift up/down
//         ladyBrown.move_absolute(1800, -200);
//         ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

//     } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
//         ladyBrown.move_absolute(0, 200);
//         ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

//     } else {
//         ladyBrown.move_absolute(0, 250);
//     }
// }

//     lemlib::PID(0, 0, 0.5, false);

//     void updateLadyTask(double target) {
//         double current = rotationSensor.get_position();

//         double error = ladyBrown.calculate(target, current);

//         ladyBrown.move(error);
//     }
// }