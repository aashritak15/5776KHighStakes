#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "intake.hpp"
#include "globals.hpp"
#include <iostream>

int intakeState = 0;
int sortState; //1 = score blue sort red, 2 = score red sort blue
//int sortState1 = 0;
bool buttonUpPressed = false;
bool colorDetected = false;

// bool buttonUpPressed = false;
// bool colorDetected = false;

void colorSort(int lol) {
    while (true) {
        if(sortState == 1) {
            if(optical.get_hue() < 30 && optical.get_hue() > 8) {
                if(!colorDetected) {
                    colorDetected = true;
                    
                    if(intake.get_actual_velocity() >= 500) {
                        pros::Task::delay(40);
                        intake.move_voltage(0);
                        pros::Task::delay(260);
                        intake.move_voltage(-12000);
                    } else {
                        intake.move_voltage(12000);
                        pros::Task::delay(0);
                        intake.move_voltage(-12000);
                    }
                }
            } else {
                colorDetected = false;
            }
            
        } else if(sortState == 2) {
            if(optical.get_hue() < 245 && optical.get_hue() > 90) {
                if(!colorDetected) {
                    colorDetected = true;
                    
                    if(intake.get_actual_velocity() >= 500) {
                        pros::Task::delay(40);
                        intake.move_voltage(0);
                        pros::Task::delay(260);
                        intake.move_voltage(-12000);
                    } else {
                        intake.move_voltage(12000);
                        pros::Task::delay(0);
                        intake.move_voltage(-12000);
                    }
                }
            } else {
                colorDetected = false;
            }
        }

        pros::delay(10);
    }

}

void intakeInit() { 
    intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST); 
    sortState = 0; //TODO: CHANGE

    void* colorSorted = (void*)colorSort;

    pros::task_fn_t colorSortation = (pros::task_fn_t) colorSorted;

    pros::Task sortTask (colorSortation);
}

void antiJamInit() { 
    void* antiJammed = (void*)antiJam;

    pros::task_fn_t antiJamtation = (pros::task_fn_t) antiJammed;

    pros::Task jamTask (antiJamtation);
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
                intake.move_voltage(12000);
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
                intake.move_voltage(-12000);
            } else if (intakeState == 2) {
                intakeState = 0;
                intake.move_voltage(0);
            }
        }
    } else {
        buttonxPressed = false;
    }
}

void updateColorSort() {
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
        if(!buttonUpPressed) {
            buttonUpPressed = true;
            if(sortState == 0) {
                sortState = 1;
                controller.set_text(0, 0, "scores blue      ");
            } else if(sortState == 1) {
                sortState = 2;
                controller.set_text(0, 0, "scores red       ");
            } else if(sortState == 2) {
                sortState = 0;
                controller.set_text(0, 0, "no sort         ");
            }
        }
    } else {
        buttonUpPressed = false;
    }

    /*
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
        if (!buttonUpPressed) {
            buttonUpPressed = true;
            if (sortState1 == 0 || sortState1 == 2) {
                sortState1 = 1;
                sortState = 1
            } else if (sortState1 == 1) {
                sortState1 = 0;
                sortState = 0;
            }
        }
    } else {
        buttonUpPressed = false;
    }
    */
}

void antiJam() {
    while (true) {
        if(intake.get_actual_velocity() < 0 && intake.get_current_draw() > 0) {
            intake.move_voltage(12000);
            pros::Task::delay(40);
            intake.move_voltage(-12000);
        }
    }
}