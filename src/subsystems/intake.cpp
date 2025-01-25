#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "ports.hpp"
#include "intake.hpp"
#include "intakeFirst.hpp"
#include "globals.hpp"
#include <iostream>

int intakeState = 0;
int sortState = 2; //1 = score blue color sort red, 2 = score red color sort blue
int prevSortState = 0;
int stateState = 0;

// bool buttonUpPressed = false;
// bool colorDetected = false;

void colorSort(int lol) {

    static bool buttonUpPressed = false;
    static bool colorDetected = false;

    while (true) {
        // if(!buttonUpPressed) {
        //     buttonUpPressed = true;
        //     if(sortState == 0) {
        //         sortState = 1;
        //         controller.set_text(0, 0, "scores red ");
        //     } else if(sortState == 1) {
        //         sortState = 2;
        //         controller.set_text(0, 0, "scores blue");
        //     } else if(sortState == 2) {
        //         sortState = 0;
        //     }
        // } else {
        //     buttonUpPressed = false;
        // }

        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
            sortState = 0;
            controller.set_text(0, 0, "TANISH YOU rock!   ");
        }

        if(sortState == 1)
            controller.set_text(0, 0, "scores blue  ");
        else if(sortState == 2)
            controller.set_text(0, 0, "scores red    ");

        //     if (!buttonUpPressed) {
        //         buttonUpPressed = true;
        //         if (stateState == 0 || stateState == 2) {
        //             stateState = 1;
        //             prevSortState = sortState;
        //             sortState = 0;
        //             controller.set_text(0, 0, "no sort      ");
        //         } else if (stateState == 1) {
        //             stateState = 0;
        //             sortState = prevSortState;
        //             // if(sortState == 1)
        //             //     controller.set_text(0, 0, "scores blue  ");
        //             // else if(sortState == 2)
        //             //     controller.set_text(0, 0, "scores red    ");
        //         }
        //     } else {
        //         buttonUpPressed = false;
        //     }
        // }
                    //std::cout<<optical.get_hue()<<"\n";

        if(sortState == 1) {
            
            if(optical.get_hue() < 30 && optical.get_hue() > 8) {
                if(!colorDetected) {
                    colorDetected = true;
                    pros::Task::delay(40);
                    intake.move_voltage(0);
                    pros::Task::delay(260);
                    intake.move_voltage(-12000);
                    std::cout<<optical.get_hue()<<"kjfjhgjhg\n";
                }
            } else {
                colorDetected = false;
            }
            //std::cout<<colorDetected<<"\n";
        } else if(sortState == 2) {
            if(optical.get_hue() < 245 && optical.get_hue() > 90) {
                if(!colorDetected) {
                    colorDetected = true;
                    pros::Task::delay(40);
                    intake.move_voltage(0);
                    pros::Task::delay(260);
                    intake.move_voltage(-12000);
                    std::cout<<optical.get_hue()<<"kjfjhgjhg\n";
                }
            } else {
                colorDetected = false;
            }
        } else {
            //pros::lcd::print(4, "NAY");
        }

        pros::delay(10);
    }

}

void intakeInnit() { 
    intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST); 

    void* colorSorted = (void*)colorSort;

    pros::task_fn_t colorSortation = (pros::task_fn_t) colorSorted;

    pros::Task sortTask (colorSortation);
}

void opticalInit() { optical.set_led_pwm(100); }

void updateIntake() {
    static bool buttonl1Pressed = false;
    static bool buttonxPressed = false;
   
    // bool buttonaPressed = false;

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        if (!buttonl1Pressed) {
            buttonl1Pressed = true;
            if (intakeState == 0 || intakeState == 2) {
                intakeState = 1;
                intake.move_voltage(-12000);
            } else if (intakeState == 1) {
                intakeState = 0;
                intake.move_voltage(0);
            }
        }
    } else {
        buttonl1Pressed = false;
    }

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        if (!buttonxPressed) {
            buttonxPressed = true;
            if (intakeState == 0 || intakeState == 1) {
                intakeState = 2;
                intake.move_voltage(12000);
            } else if (intakeState == 2) {
                intakeState = 0;
                intake.move_voltage(0);
            }
        }
    } else {
        buttonxPressed = false;
    }
}

// void sortTest() {
//     static bool colorDetected = false;

//     if(optical.get_hue() < 18 && optical.get_hue() > 12) { //TUNE
//         if(!colorDetected) {
//             colorDetected = true;
//             intake.move_voltage(0);
//             pros::Task::delay(200);
//             intake.move_voltage(-12000);
            
//             // intakeState = 3;
//             // pros::delay(200);
//             // intake.move_voltage(0);
//             // pros::delay(100);
//             // intake.move_voltage(-12000);
//             // intakeState = 1;
//         }
//     } else {
//         colorDetected = false;
//     }
// }

// void updateColorToggle() {
//     static bool leftPressed = false;
//     static bool rightPressed = false;

//     if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
//         if(!leftPressed) {
//             leftPressed = true;
//             if(sortState == 0 || sortState == 2) {
//                 sortState = 1;
//             } else if (sortState == 1) {
//                 sortState = 0;
//             }
//         }
//     } else {
//         leftPressed = false;
//     }

//     if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
//         if(!rightPressed) {
//             rightPressed = true;
//             if(sortState == 0 || sortState == 1) {
//                 sortState = 2;
//             } else if (sortState == 2) {
//                 sortState = 0;
//             }
//         }
//     } else {
//         rightPressed = false;
//     }
// }

// void colorSort() {
//     static bool colorDetected = false;

//     if(sortState == 1) {
//         if(optical.get_hue() < 20 && optical.get_hue() > 8) { //red!
//             if(!colorDetected) {
//                 colorDetected = true;
//                 intakeState = 3;
//                 pros::Task::delay(70);
//                 intake.move_voltage(0);
//                 pros::Task::delay(250);
//                 intake.move_voltage(-12000);
//                 intakeState = 1;
//             }
//         } else {
//             colorDetected = false;
//         }
//     } else if(sortState == 2) {
//         if(optical.get_hue() < 225 && optical.get_hue() > 210) { //blue!
//             if(!colorDetected) {
//                 colorDetected = true;
//                 intakeState = 3;
//                 pros::Task::delay(70); //TODO: is the task delay working
//                 intake.move_voltage(0);
//                 pros::Task::delay(250);
//                 intake.move_voltage(-12000);
//                 intakeState = 1;
//             }
//         } else {
//             colorDetected = false;
//         }
//     }
// }

// void resetIntake() {
//     // if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
//     //     //intake loop value:
//     //     int onethird;
//     //     if(intake.get_position()/onethird == 0) {
//     //         intake.move_absolute(0, 200);
//     //     } else if(intake.get_position()/onethird == 1) {
//     //         intake.move_absolute(onethird, 200);
//     //     } else if(intake.get_position()/onethird == 2) {
//     //         intake.move_absolute(2*onethird, 200);
//     //     }
//     // }

//     if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
//         if (intake.get_position() / 120 == 0) {
//             intake.move_absolute(0, 200);
//         } else if (intake.get_position() / 120 == 1) {
//             intake.move_absolute(120, 200);
//         } else if (intake.get_position() / 120 == 2) {
//             intake.move_absolute(240, 200);
//         }
//     }
// }

// void stepIntake() {
//     int onethird;
//     if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) { intake.move_absolute(onethird, 200); }
// }
