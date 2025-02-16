#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "ladybrown.hpp"
#include "magic.hpp"
#include "pros/rtos.h"
#include <cmath>

ASSET(redMogoAlliancePath_txt); // TODO: add std functionality
ASSET(redMogoAllianceExtra_txt);
ASSET(autonomous_txt); // TODO: add std functionality
ASSET(extra_txt);
ASSET(blueMogoAlliancePath_txt);

// ASSET(blueMogoAlliancePath_txt); // TODO: add std functionality
// ASSET(blueMogoAllianceExtra_txt);

void redMogoAlliance() {
    sortState = 2;
    chassis.follow(blueMogoAlliancePath_txt, redMogoAllianceExtra_txt, "blue mogo alliance");
    //TODO: DOESN'T RUN RED
}

// void blueMogoAlliance() {
//     sortState = 1;
//     chassis.follow(blueMogoAlliancePath_txt, blueMogoAllianceExtra_txt, "blue mogo alliance");
// }

// rd::Selector selector({
//     {"red mogo alliance", &redMogoAlliance}
//     // {"blue mogo alliance", &blueMogoAlliance},
// });

// rd::Console console;


void initialize() {
    // pros::lcd::initialize();

    

    // selector.focus();

    // selector.on_select([](std::optional<rd::Selector::routine_t> routine) {
	// 	if (routine == std::nullopt) {
	// 		std::cout << "No routine selected" << std::endl;
	// 	} else {
	// 		std::cout << "Selected Routine: " << routine.value().name << std::endl;
	// 	}
	// });

    // if (pros::lcd::is_initialized()) { std::cout << "yippeee\n"; }

    // clampInit();
    // doinkInit();
    // intakeInit();
    // ladyBrownInit();

    // chassis.calibrate(true);
    // chassis.setPose(0, 0, 0);

    // pros::Task screenTaskation(screenTask, "screen task");
}

// Runs while the robot is disabled
void disabled() {
    
}

// Runs after initialize if the robot is connected to field control
void competition_initialize() {
    
}

void redMogo() {
    // mogo
    int sortState = 2;
    chassis.moveToPoint(0, -27, 2000, {.forwards = false, .maxSpeed = 50});
    chassis.waitUntilDone();
    mogoClamp.set_value(true);

    pros::delay(250);
    intake.move_voltage(12000);
    pros::delay(250);

    // // ring
    chassis.turnToHeading(-93, 1000);
    chassis.moveToPoint(-21, -30, 1000);
    pros::delay(2000);

    // // //ladder
    chassis.turnToHeading(-256.4, 1000);
    chassis.waitUntilDone();

    mogoClamp.set_value(false);
    //globalTarget = 180;
    //chassis.moveToPoint(-7, -38, 1000);

    // globalTarget = 1000;

    // // intake.move_voltage(0);

    // // elims
    // chassis.turnToHeading(-233.7, 2000);
    // chassis.moveToPoint(-37, -19.6, 2000, {.forwards = false});
    // chassis.waitUntilDone();
    // mogoClamp.set_value(false);
    // intake.move_voltage(0);
    // chassis.moveToPoint(-24.5, -33, 2000);
    // chassis.turnToHeading(7, 2000);
}

void blueMogo() { //blue
    // mogo
    int sortState = 1;
    chassis.moveToPoint(0, -27, 2000, {.forwards = false, .maxSpeed = 50});
    chassis.waitUntilDone();
    mogoClamp.set_value(true);

    pros::delay(250);
    intake.move_voltage(12000);
    pros::delay(250);

    // // ring
    chassis.turnToHeading(93, 1000);
    chassis.moveToPoint(23, -27, 1000);
    pros::delay(2000);

    // // //ladder
    chassis.turnToHeading(-256.4, 1000);
    chassis.waitUntilDone();

    mogoClamp.set_value(false);
    globalTarget = 180;

    //
    chassis.moveToPoint(0, -40, 1000);


    // globalTarget = 1000;

    // // intake.move_voltage(0);

    // // elims
    // chassis.turnToHeading(-233.7, 2000);
    // chassis.moveToPoint(-37, -19.6, 2000, {.forwards = false});
    // chassis.waitUntilDone();
    // mogoClamp.set_value(false);
    // intake.move_voltage(0);
    // chassis.moveToPoint(-24.5, -33, 2000);
    // chassis.turnToHeading(7, 2000);
}

void autonomous() {

    chassis.follow(blueMogoAlliancePath_txt, redMogoAllianceExtra_txt, "blue mogo alliance");
    
    // chassis.calibrate();
    // chassis.setPose(0, 0, 0);
    // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    // // TODO: COMMENTED OUT BC TESTING IN INITIALIZE
    //initDebug();

    //blueMogo();
    // redMogo();

    //chassis.follow(redMogoAlliancePath_txt, redMogoAllianceExtra_txt, "red mogo alliance");

    // redMogo2();
    // blueMogo1();

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

    // reflect(false, true);

    // interrupt
    //  chassis.follow(autonomous_txt, extra_txt, "red");

    // rerun
    // initO();

    // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    // int count = 1;

    // while (true) {
    //     int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    //     int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

    //     chassis.arcade(leftY, rightX); // 0.9

    //     updateIntake();
    //     updateColorSort();
    //     updateClamp();
    //     updateDoink();
    //     updateLB();
    //     // lbTask();

    //     if (count == 5) { //*data written every 0.1 seconds
    //         writePose();
    //         writeAdditional();
    //         count = 1;
    //         fileO.flush();
    //         fileOTwo.flush();

    //         // writeInterruptPose(); //*INTERRUPT
    //         // writeInterruptAdditional();
    //         // count = 1;
    //         // fileInterrupt.flush();
    //         // fileInterruptTwo.flush();
    //     }
    //     count++;

    //     // closeO();
    //     // closeOInterrupt(); //*INTERRUPT

    //     pros::delay(10);
    // }

    // drive
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    while (true) {
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        chassis.arcade(leftY, rightX); // 0.9

        updateIntake();
        updateColorSort();
        updateClamp();
        updateDoink();
        updateLB();
        // lbTask();

        pros::delay(10);
    }
}