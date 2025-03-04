#include "intake.hpp"
#include "piston.hpp"
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

// rd::Selector selector("Auton Selector", autonRoutines);

void initialize() {
    // selector.focus();
    //  console.focus();
    //   console.printf("Initializing...\n");
    chassis.calibrate();

    chassis.setPose(0, 0, 0);

    pros::lcd::initialize();

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

    pros::Task screenTask([&]() {
        while (true) {
            // print robot location to the brain screen

            //     std::vector<double> left = leftMotors.get_position_all();
            // std::vector<double> right = rightMotors.get_position_all();
            // pros::lcd::print(5, "LeftF Encoders: %f", left[0]);
            // pros::lcd::print(6, "LeftM Encoders: %f", left[1]);
            // //pros::lcd::print(7, "LeftB Encoders: %f", left[2]);
            // pros::lcd::print(2, "RightF Encoders: %f", right[0]);
            // pros::lcd::print(3, "RightM Encoders: %f", right[1]);
            // pros::lcd::print(4, "RightB Encoders: %f", right[2]);

            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            pros::lcd::print(3, "LB: %f", ladyBrown.get_position()); // lift encoder
            pros::lcd::print(4, "Color: %f ", optical.get_hue());

            // log position telemetry
            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
            // delay to save resources
            pros::delay(50);
        }
    });

    // screenTask();

    // screenInit();
}

// Runs while the robot is disabled
void disabled() {}

// Runs after initialize if the robot is connected to field control
void competition_initialize() {} // selector.focus(); }}

ASSET(ringsideRed_txt);
ASSET(ringsideBlue_txt);
ASSET(ringsideExtra_txt);
ASSET(skillsPath_txt);
ASSET(skillsExtra_txt);
ASSET(autonomous_txt);
ASSET(extra_txt);

void skills() {
    //alliance
    intakeState = 1; 
    pros::delay(500);
    intakeState = 0;

    //mogo
    chassis.moveToPoint(0, 11, 500);
    chassis.turnToHeading(90, 500);
    chassis.moveToPoint(-25, 12.3, 750, {.forwards = false});
    chassis.waitUntilDone();
    clampState = 1;
    pros::delay(250);

    //mogo ring #1
    chassis.turnToHeading(0, 500);
    intakeState = 1;
    chassis.moveToPoint(-25, 36, 500);
    pros::delay(250);

    //wall stake ring #1
    chassis.turnToHeading(-30.5, 500);
    chassis.waitUntilDone();
    pros::delay(250);
    globalTarget = 26;
    chassis.moveToPoint(-48.5, 70.9, 1000);
    chassis.waitUntilDone();
    pros::delay(1000);
    intakeState = 0;

    //back up and go to wall stake
    chassis.moveToPose(-34.1, 37.8, -51.6, 100000, {.forwards = false, .lead = 0.5, .minSpeed = 50});
    chassis.moveToPose(-50, 49.5, -84, 100000, {.lead = 0.7, .minSpeed = 50});
    chassis.moveToPose(-60.3, 49.5, -90, 750);
    chassis.waitUntilDone();
    
    //score wall stake and get mogo ring #2
    intakeState = 2;
    pros::delay(25);
    globalTarget = 80;
    pros::delay(500);
    intakeState = 1;
    chassis.moveToPoint(-63.8, 49.5, 500);
    chassis.waitUntilDone();
    globalTarget = 140;
    pros::delay(1000);

    //back up and get mogo rings #3 4 5
    chassis.moveToPoint(-48, 49.5, 500, {.forwards = false});
    chassis.waitUntilDone();
    globalTarget = 0;
    chassis.turnToHeading(-178, 500);
    chassis.moveToPose(-48.4, 32.4, 180, 500);
    // chassis.waitUntilDone();
    // chassis.moveToPose(-48.5, 13.14, 180, 500);
    // chassis.waitUntilDone();
    chassis.moveToPose(-48.6, 0.2, 180, 500);
    chassis.waitUntilDone();

}

void autonomous() {
    chassis.setBrakeMode(MOTOR_BRAKE_BRAKE);
    sortState = 2;
    skills();

    // fourRingRed();

    // fourRingBlue();

    //chassis.follow(ringsideRed_txt, ringsideExtra_txt, "ringside");
    //chassis.follow(skillsPath_txt, skillsExtra_txt, "skills");
    // chassis.follow(autonomous_txt, extra_txt, "test");

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

    sortState = 0;
    matchControl();

    // sortState = 1;
    // rerunControl();

    // *INTERRUPT
    //chassis.follow(skillsPath_txt, skillsExtra_txt, "skills");
    //rerunControl();

    //*REFLECT
    // reflect(true, false);
}
