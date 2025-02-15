#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "intake.hpp"
#include "globals.hpp"
#include <iostream>

int intakeState = 0;
int sortState; // 1 = score blue sort red, 2 = score red sort blue
bool buttonUpPressed = false;
bool colorDetected = false;

// bool buttonUpPressed = false;
// bool colorDetected = false;

void intakeInit() {
    intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    optical.set_led_pwm(100);

    pros::Task pd_task2(colorSort, "color sort");
    // pros::Task pd_task3(antiJam);
}

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
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
        if (!buttonUpPressed) {
            buttonUpPressed = true;
            if (sortState == 0) {
                sortState = 1;
                controller.set_text(0, 0, "scores blue      ");
                pros::lcd::print(6, "Score blue");
            } else if (sortState == 1) {
                sortState = 2;
                controller.set_text(0, 0, "scores red       ");
                pros::lcd::print(6, "Score red");
            } else if (sortState == 2) {
                sortState = 0;
                controller.set_text(0, 0, "no sort         ");
                pros::lcd::print(6, "No sort");
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

void colorSort() {
    while (true) {
        if (sortState == 1) {
            if (optical.get_hue() < 30 && optical.get_hue() > 0) {
                if (!colorDetected) {
                    colorDetected = true;

                    // if (intake.get_actual_velocity() >= 200) {
                    pros::Task::delay(65);
                    intake.move_voltage(-12000);
                    pros::Task::delay(200);
                    intake.move_voltage(12000);
                    // } else {
                    //     intake.move_voltage(-12000);
                    //     pros::Task::delay(750);
                    //     intake.move_voltage(12000);
                    // }
                }
            } else {
                colorDetected = false;
            }

        } else if (sortState == 2) {
            if (optical.get_hue() < 245 && optical.get_hue() > 200) {
                if (!colorDetected) {
                    colorDetected = true;

                    // if (intake.get_actual_velocity() >= 200) {
                    pros::Task::delay(65);
                    intake.move_voltage(-12000);
                    pros::Task::delay(200);
                    intake.move_voltage(12000);
                    // } else {
                    //     intake.move_voltage(-12000);
                    //     pros::Task::delay(750);
                    //     intake.move_voltage(12000);
                    // }
                }
            } else {
                colorDetected = false;
            }
        }

        pros::delay(10);
    }
}

void antiJam() {
    while (true) {
        std::cout << "Intake Current: " << std::to_string(intake.get_current_draw()) << "\n";

        // Detect a jam based on velocity and current draw
        if (intake.get_actual_velocity() < 100 && intake.get_current_draw() > 2400) {
            intake.move_voltage(12000); // Try to push forward for a moment
            pros::Task::delay(40);
            intake.move_voltage(-12000); // Reverse to clear jam
            pros::Task::delay(200); // Hold reverse for longer in case of severe jam
            intake.move_voltage(6000); // Resume with reduced power before full power
            pros::Task::delay(100);
            intake.move_voltage(12000);
        }

        pros::delay(10);
    }
}