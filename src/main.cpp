#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "ladybrown.hpp"
#include "magic.hpp"
#include "autons.hpp"
#include <cmath>
#include "main.h"
#include "loops.hpp"

#include "autonSelector.hpp"

// std::vector<rd::Selector::routine_t> autonRoutines = {
//     {"Blue Auton", []() { blueMogo(); }, "", 210}, // Blue color hue
//     {"Red Auton", []() { redMogo(); }, "", 0}, // Red color hue

// };

//rd::Selector selector("Auton Selector", autonRoutines);

void initialize() {
    // selector.focus();
    //  console.focus();
    //   console.printf("Initializing...\n");
    chassis.calibrate();

    chassis.setPose(0, 0, 0);

    // pros::delay(250);

    // selector.on_select([](std::optional<rd::Selector::routine_t> routine) {
    //     if (routine == std::nullopt) {
    //         std::cout << "No routine selected" << std::endl;
    //     } else {
    //         std::cout << "Selected Routine: " << routine.value().name << std::endl;
    //     }
    // });

    clampInit();
    intakeInit();
    ladyBrownInit();
    controller.set_text(0, 0, "initialized");

    pros::Task screenTaskActual(screenTask, "screen task");
}

// Runs while the robot is disabled
void disabled() {}

// Runs after initialize if the robot is connected to field control
void competition_initialize() {} // selector.focus(); }}

ASSET(redFiveRingAuton_txt);
ASSET(redFiveRingExtra_txt);

void fourRingRed() {
    chassis.setPose(0, 0, 0);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    chassis.moveToPoint(0.2, -29, 2000, {.forwards = false, .maxSpeed = 60}); // go to mogo

    chassis.waitUntilDone();
    clampState = 1;
    pros::delay(500);

    chassis.turnToHeading(-90, 1000);

    intakeState = 1;

    pros::delay(250);

    chassis.moveToPoint(-22.4, -31, 2000); // intake ring # 1
    chassis.waitUntilDone();

    pros::delay(250);

    chassis.turnToHeading(0, 1000);

    chassis.moveToPoint(-23, -11.5, 1000);

    pros::delay(500);

    chassis.turnToHeading(-44, 1000); // turn to face ring stack

    chassis.moveToPoint(-28.71, -5.99, 1000);
    chassis.waitUntilDone();
    intakeState = 2;
    chassis.moveToPoint(-47.6, 13.55, 2000, {.minSpeed = 100});
    chassis.waitUntilDone(); // go to ring stack

    pros::delay(100);
    intakeState = 1;

    pros::delay(500);
    chassis.moveToPoint(-28.71, -5.99, 1000, {.forwards = false}); // go back
    chassis.waitUntilDone();
    intakeState = 2;
    chassis.moveToPoint(-47.6, 13.55, 2000, {.minSpeed = 80});
    chassis.waitUntilDone(); // go to ring stack

    pros::delay(100);
    intakeState = 1;

    pros::delay(500);
    chassis.moveToPoint(-28.71, -5.99, 1000, {.forwards = false}); // go back

    // chassis.turnToHeading(-266, 1000); // turn to face ladder

    // chassis.moveToPoint(-1.23, -43, 6000); // go to the ladder
}

void autonomous() {
    sortState = 2;

    // fourRingRed();
    chassis.follow(redFiveRingAuton_txt, redFiveRingExtra_txt, "red five ring");
    // void fourRingRed();

    // selector.run_auton();
    // initDebug();
    // chassis.calibrate();
    // chassis.setPose(0, 0, 0);
    // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    // // TODO: COMMENTED OUT BC TESTING IN INITIALIZE

    // std::cout << "running auton\n";
    // chassis.turnToHeading(90, 3000);
    // chassis.moveToPoint(0, 24, 3000);
    // chassis.follow(autonomous_txt, extra_txt, "red solo wp");

    // chassis.follow(blueMogoAlliancePath_txt, redMogoAllianceExtra_txt, "blue mogo alliance");

    // blueRing();
    // blueMogo();
    // redMogo();

    // chassis.follow(redMogoAlliancePath_txt, redMogoAllianceExtra_txt, "red mogo alliance");

    // if(color == 0) { //red
    //     sortState = 2
    //     switch(auton) {
    //         case 1: //solo wp
    //             chassis.follow(autonomous_txt, extra_txt, "adsf");
    //             break;
    //         case 2: //mogo rush
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 3:
    //             chassis.moveToPoint(0, 24, 10000);
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 4:
    //             chassis.turnToHeading(90, 10000);
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 5:
    //             chassis.turnToHeading(180, 10000);
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 6:
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 7:
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 8:
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //     }
    // } else if(color == 1) { //blue
    //     sortState = 1;
    //     switch(auton) {
    //         case 1: //solo wp
    //             chassis.follow(autonomous_txt, extra_txt, "adsf");
    //             break;
    //         case 2: //mogo rush
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 3:
    //             chassis.moveToPoint(0, 24, 10000);
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 4:
    //             chassis.turnToHeading(90, 10000);
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 5:
    //             chassis.turnToHeading(180, 10000);
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 6:
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 7:
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //         case 8:
    //             //chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);
    //             break;
    //     }
    // }
    // chassis.turnToHeading(180, 5000);
}

void opcontrol() {
    // selector.focus();
    rerunControl();

    // printCoords();

    // matchControl();
}