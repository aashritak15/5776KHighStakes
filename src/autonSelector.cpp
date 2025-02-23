#include "globals.hpp"
#include "autons.hpp"
#include "autonSelector.hpp"
#include "piston.hpp"
#include "intake.hpp"
#include "ladybrown.hpp"

// add color sort after works
rd::Console console;

void printCoords() {
    console.printf("X: %f", chassis.getPose().x);
    console.printf("Y: %f", chassis.getPose().y);
    console.printf("Theta: %f", chassis.getPose().theta);
    console.printf("Color: %f", optical.get_hue());
    console.printf("LB Rot Sensor: %i", lbRotation.get_position());
    console.printf("Intake vel: %lf", intakeLower.get_actual_velocity());
}

void screenTask() {
    while (true) {
        //* line 7 reserved for rerun states.
        pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
        pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
        pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
        pros::lcd::print(3, "Color: %f", optical.get_hue());
        pros::lcd::print(4, "LB Rot Sensor: %i", lbRotation.get_position());
        pros::lcd::print(5, "Intake vel: %lf", intakeLower.get_actual_velocity());
        pros::Task::delay(50);
    }
}