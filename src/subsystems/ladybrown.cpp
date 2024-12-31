#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ports.hpp"
#include "ladybrown.hpp"
#include "globals.hpp"

pros::Rotation rotationSensor(13);

void liftInit() { lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); }
void rotationInit() { }

void updateLift() {
    lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { // lift up/down
        lift.move_voltage(-12000*0.55);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { // lift up/down
        lift.move_voltage(12000*0.55);
    } else {
        lift.move_voltage(0);
    }

    /*
       if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { //lift up/down
            lift.move_velocity(-100);
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { //lift up/down
            lift.move_velocity(100);
        } //else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            //lift.move_absolute(143,40);
        else {lift.move_velocity(0);}*/

    //lb macros
    int leftButton = 0;
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        if (leftButton == 0) {
            while(rotationSensor.get_angle()!=2583)
                lift.move_voltage(-12000*0.55);
            lift.move_voltage(0);
            //lift.move_absolute(25.83, 100);
            leftButton++;
        } else if (leftButton == 2) {
            while(rotationSensor.get_angle()!=15000)
                lift.move_voltage(-12000*0.55);
            lift.move_voltage(0);
            //lift.move_absolute(150, 100);
            leftButton++;
        } else if (leftButton == 4) {
            while(rotationSensor.get_angle()!=0)
                lift.move_voltage(12000*0.55);
            lift.move_voltage(0);
            //lift.move_absolute(0, 100);
            leftButton++;
        }
    } else if (!controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        if (leftButton == 1 || leftButton == 3) {
            leftButton++;
        } else if (leftButton == 5) {
            leftButton = 0;
        }
    }
}


// void rotationInit() { }

//     /* 
//         4 stage toggle:

//         1. 20 
//         2. 29
//         3. 143
//         4. 0

//     */

// bool checkIfRotation(){
//     return abs(lift.get_actual_velocity() > 5);
// }

// double kp = 0;

// double kd = 0;

// double ki = 0; //dont need this i think 


// // void rotationPID(double degrees){
// //     while(true){

// //     }
// // }

    



//    int ladyState = 0;


//    //temporary function until I finish the pid 

//    void ladyBrown() {

//         double rotation_value = rotationSensor.get_angle();
//         if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
//             if( ladyState == 0){

//                 lift.move_velocity(10);
                
//                 pros::delay(10);
//                 lift.move_voltage(0);

//                 while(rotation_value != 20){
//                     lift.move_velocity(-30);
//                 }z

//                 lift.move_velocity(0);

//                 ladyState++;
                
//             }

//             else if( ladyState == 1){

                
//                 while(rotation_value != 29){
//                     lift.move_velocity(-30);
//                 }

//                 lift.move_velocity(0);

//                 ladyState++;
                

//             }


//         }
    
//     else if(!controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
//             if(ladyState == 1){
//                 ladyState++;
//             }
//              if(ladyState == 3){
//                 ladyState == 0;
//             }

            
//     }

//    }
