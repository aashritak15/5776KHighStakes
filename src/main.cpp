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
            std::cout << chassis.getPose().x << ", " << chassis.getPose().y << ", " << chassis.getPose().theta << "\n";

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

void fourRingRed() {
    chassis.setPose(0, 0, 0);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    chassis.moveToPoint(0.2, -29, 2000, {.forwards = false}); // go to mogo

    chassis.waitUntilDone();
    clampState = 1;
    pros::delay(500);

    chassis.turnToHeading(-90, 800);

    intakeState = 1;

    pros::delay(250);

    chassis.moveToPoint(-22.4, -31, 2000); // intake ring # 1
    chassis.waitUntilDone();

    // pros::delay(250);

    chassis.turnToHeading(0, 1000);

    chassis.moveToPoint(-23, -11.5, 750);

    pros::delay(600);

    chassis.turnToHeading(-44, 800); // turn to face ring stack

    // ram 1
    // chassis.moveToPoint(-30.71, -3.99, 1000);
    // chassis.waitUntilDone();

    pros::delay(1000);

    chassis.moveToPoint(-45.6, 11.55, 1050, {.minSpeed = 50});
    intakeState = 2;
    chassis.waitUntilDone(); // go to ring stack
    pros::delay(100);
    intakeState = 1;

    // ram 2
    pros::delay(500);
    chassis.moveToPoint(-32.71, -1.99, 1000, {.forwards = false}); // go back
    chassis.waitUntilDone();
    intakeState = 2;
    chassis.moveToPoint(-45.6, 11.55, 750, {.minSpeed = 50});
    chassis.waitUntilDone(); // go to ring stack
    pros::delay(100);
    intakeState = 1;

    // ram 3
    pros::delay(500);
    chassis.moveToPoint(-32.71, -1.99, 1000, {.forwards = false}); // go back
    chassis.waitUntilDone();
    intakeState = 2;
    chassis.moveToPoint(-45.6, 11.55, 750, {.minSpeed = 50});
    chassis.waitUntilDone(); // go to ring stack
    pros::delay(100);
    intakeState = 1;

    pros::delay(500);
    chassis.moveToPoint(-30.71, -3.99, 1000, {.forwards = false, .minSpeed = 100}); // go back

    chassis.turnToHeading(134, 800); // turn to face ladder

    intakeState = 0;
    // chassis.waitUntilDone();
    // clampState = 0;

    chassis.moveToPoint(-1.52, -21.3, 6000); // go to the ladder

    globalTarget = 140;
}

void fourRingBlue() {
    chassis.setPose(0, 0, 0);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    chassis.moveToPoint(-0.2, -29, 1000, {.forwards = false}); // go to mogo

    chassis.waitUntilDone();
    clampState = 1;
    pros::delay(500);

    chassis.turnToHeading(90, 800);

    intakeState = 1;

    chassis.moveToPoint(18.3, -27.6, 800); // intake ring # 1
    chassis.waitUntilDone();

    // pros::delay(250);

    chassis.turnToHeading(0, 1000);

    chassis.moveToPoint(17.7, -5.2, 750);

    // pros::delay(600);

    chassis.turnToHeading(47, 800); // turn to face ring stack

    pros::delay(250);

    // // ram 1
    chassis.moveToPoint(29.9, 5.9, 1000);

    intakeState = 2;
    chassis.waitUntilDone(); // go to ring stack
    pros::delay(100);
    intakeState = 1;

    // ram 2
    pros::delay(500);
    chassis.moveToPoint(21.2, -2.6, 750, {.forwards = false}); // go back
    chassis.waitUntilDone();
    intakeState = 2;
    chassis.moveToPoint(29.9, 5.9, 1000);
    chassis.waitUntilDone(); // go to ring stack
    pros::delay(100);
    intakeState = 1;

    // ram 3
    pros::delay(500);
    chassis.moveToPoint(21.2, -2.6, 750, {.forwards = false}); // go back
    chassis.waitUntilDone();
    intakeState = 2;
    chassis.moveToPoint(30.9, 6.9, 750);
    chassis.waitUntilDone(); // go to ring stack
    pros::delay(100);
    intakeState = 1;

    pros::delay(500);
    chassis.moveToPoint(21.71, 2.99, 750, {.forwards = false, .minSpeed = 100}); // go back

    chassis.turnToHeading(243.9, 800); // turn to face ladder

    intakeState = 0;
    // chassis.waitUntilDone();
    // clampState = 0;

    chassis.moveToPoint(-19, -24, 3000);

    // // globalTarget = 140;
}

void autonomous() {
    sortState = 1;

    // fourRingRed();

    fourRingBlue();

    // chassis.follow(ringsideRed_txt, ringsideExtra_txt, "ringside");
    //  chassis.follow(blueFiveRingAuton_txt, fiveRingExtra_txt, "ringside");
    // chassis.follow(skillsPath_txt, skillsExtra_txt, "skills");

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
    // matchControl();
    rerunControl();

    // *INTERRUPT
    // chassis.follow(ringsideRed_txt, ringsideExtra_txt, "ringside");
    // rerunControl();

    //*REFLECT
    // reflect(true, false);
}