#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ports.hpp"
#include "ladybrown.hpp"
#include "globals.hpp"

pros::Rotation rotationSensor(13);

void liftInit() { lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); }

// const int position1 = 24;
// const int position2 = 100; // Adjust as per your desired positions
// const int position3 = 0;

// double kp = 0.0; // Set appropriate PID tuning constants
// double kd = 0.0;

// int target = position1;

// double prevError = 0;

// bool pidControl = false;

// bool ladyState = false;

// // PID Update Function
// void updateLadyPID() {
//     if (pidControl) {
//         int current = rotationSensor.get_angle();

//         double error = target - current;
//         double derivative = error - prevError;

//         prevError = error;

//         double power = ((kp * error) + (kd * derivative)) * 100;

//         lift.move_voltage(power); // Assuming move_voltage accepts values in millivolts
//     } else {
//         lift.move_voltage(0);
//     }
// }

// // Task Update Function
// void updateLadyTask() {
//     if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
//         target = position1; // Reset to position1
//         pidControl = true;
//         ladyState = false; // Reset state
//     }

//     if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
//         if (target == position1) {
//             target = position2;
//         } else if (target == position2) {
//             target = position3;
//         } else {
//             target = position1; // Wrap back to position1
//         }
//         pidControl = true; // Enable PID control
//     }
// }





// lemlib::PID  (
//     0,
//     0,
//     0
//     5, 
//     false
// );


// void updateLadyTask(double target){

//         double current = rotationSensor.get_position(); 

//         double error = lift.calculate(target, current);

//         lift.move(error);
        
// }

