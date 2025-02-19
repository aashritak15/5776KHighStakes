#include "lemlib/api.hpp"
#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "ladybrown.hpp"
#include "magic.hpp"
#include "autons.hpp"
#include <cmath>
#include "main.h"
#include "loops.hpp"

// rd::Selector selector({ //TODO: figure this out!
//     {"red mogo alliance", &redMogoAlliance}
//     // {"blue mogo alliance", &blueMogoAlliance},
// });

// rd::Console console;


void initialize() {
    pros::lcd::initialize();

    // selector.focus();

    // selector.on_select([](std::optional<rd::Selector::routine_t> routine) {
	// 	if (routine == std::nullopt) {
	// 		std::cout << "No routine selected" << std::endl;
	// 	} else {
	// 		std::cout << "Selected Routine: " << routine.value().name << std::endl;
	// 	}
	// });

    // if (pros::lcd::is_initialized()) { std::cout << "yippeee\n"; }

    clampInit();
    doinkInit();
    intakeInit();
    ladyBrownInit();

    chassis.calibrate(true);
    chassis.setPose(0, 0, 0);

    pros::Task screenTaskActual(screenTask, "screen task");
}

// Runs while the robot is disabled
void disabled() {
    
}

// Runs after initialize if the robot is connected to field control
void competition_initialize() {
    
}

void autonomous() {
    //initDebug();
    //chassis.follow(skillsPath_txt, skillsExtra_txt, "skills");

    //chassis.follow(blueMogoAlliancePath_txt, redMogoAllianceExtra_txt, "blue mogo alliance");
    
    // chassis.calibrate();
    // chassis.setPose(0, 0, 0);
    // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    // // TODO: COMMENTED OUT BC TESTING IN INITIALIZE


    //blueRing();
    //blueMogo();
    redMogo();

    //chassis.follow(redMogoAlliancePath_txt, redMogoAllianceExtra_txt, "red mogo alliance");


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

    // rerunControl();

    matchControl();
}