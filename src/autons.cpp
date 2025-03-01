#include "autons.hpp"

// void fourRingRed() {
//     chassis.setPose(0, 0, 0);
//     chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

//     chassis.moveToPoint(0.2, -29, 2000, {.forwards = false, .maxSpeed = 60}); // go to mogo

//     chassis.waitUntilDone();
//     clampState = 1;
//     pros::delay(250);

//     chassis.turnToHeading(-90, 1000);

//     intakeState = 1;

//     pros::delay(250);

//     chassis.moveToPoint(-22.4, -31, 2000); // intake ring # 1

//     chassis.turnToHeading(0, 1000);

//     chassis.moveToPoint(-23, -12, 1000);

//     pros::delay(500);

//     chassis.turnToHeading(-44, 1000); // turn to face ring stack

//     intakeState = 2;

//     // chassis.moveToPoint(-37.2, 0.9, 700);
//     // chassis.waitUntilDone(); // go to ring stack

//     // intakeState = 1;

//     // pros::delay(500);
//     // chassis.moveToPoint(-28.7, -4, 1000, {.forwards = false}); // go back

//     // chassis.turnToHeading(-266, 1000); // turn to face ladder

//     // chassis.moveToPoint(-1.23, -43, 6000); // go to the ladder
// }

void fourRingBlue() {
    chassis.setPose(0, 0, 0);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    chassis.moveToPoint(-0.2, -29, 2000, {.forwards = false, .maxSpeed = 60}); // go to mogo

    chassis.waitUntilDone();
    clampState = 1;
    pros::delay(250);

    chassis.turnToHeading(90, 1000);

    intakeState = 1;

    pros::delay(250);

    chassis.moveToPoint(22.4, -31, 2000); // intake ring # 1

    chassis.turnToHeading(0, 1000);

    chassis.moveToPoint(23, -14, 1000);

    pros::delay(500);

    chassis.turnToHeading(44, 1000); // turn to face ring stack

    intakeState = 2;

    chassis.moveToPoint(37.2, 0.9, 700);
    chassis.waitUntilDone(); // go to ring stack

    intakeState = 1;

    pros::delay(500);
    chassis.moveToPoint(28.7, -4, 1000, {.forwards = false}); // go back

    chassis.turnToHeading(266, 1000); // turn to face ladder

    chassis.moveToPoint(1.23, -43, 6000); // go to the ladder
}

void redMogo() {
    // mogo
    int sortState = 2;
    chassis.moveToPoint(0, -27, 2000, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    mogoClamp.set_value(true);

    pros::delay(250);
    intakeUpper.move_voltage(12000);
    pros::delay(250);

    // // ring
    chassis.turnToHeading(-93, 1000);
    chassis.moveToPoint(-21, -30, 1000);
    pros::delay(2000);

    // drop mogo
    chassis.turnToHeading(-216.7, 2000,
                          {.direction = lemlib::AngularDirection::CW_CLOCKWISE}); // change to counter to not hit ring
    chassis.waitUntilDone();
    mogoClamp.set_value(false);

    // third mogo
    chassis.turnToHeading(-4.7, 1000);
    chassis.moveToPoint(-31.3, -29.8, 1000, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    mogoClamp.set_value(true);
    pros::delay(500);
    intakeUpper.move_velocity(0);

    // //move to corner
    // chassis.moveToPose(-32.4, 6.7, -55, 2000, {.lead = 0.7});
    // chassis.waitUntilDone();
    // doink.set_value(true);

    // //doink
    // chassis.moveToPose(-40, 12.7, -61, 2000);
    // chassis.turnToHeading(-120, 1000);

    // //turn back and intake
    // chassis.turnToHeading(-61, 1000);
    // //intake.move_velocity(12000);
    // doink.set_value(false);
    // chassis.moveToPoint(-45, 12.7, 1000);

    // // // //ladder
    // chassis.turnToHeading(-256.4, 1000);
    // chassis.waitUntilDone();

    // mogoClamp.set_value(false);
    // globalTarget = 180;
    // chassis.moveToPoint(-7, -38, 1000);

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

void blueRing() {
    // mogo
    chassis.moveToPoint(0, -27, 2000, {.forwards = false, .maxSpeed = 50});
    chassis.waitUntilDone();
    mogoClamp.set_value(true);

    pros::delay(250);
    intakeUpper.move_voltage(12000);
    pros::delay(250);

    // // ring
    chassis.turnToHeading(-93, 1000);
    chassis.moveToPoint(-21, -30, 1000);
    pros::delay(2000);

    // // //ladder
    chassis.turnToHeading(-256.4, 1000);
    chassis.waitUntilDone();

    // mogoClamp.set_value(false);
    globalTarget = 180;
    chassis.moveToPoint(-2, -38, 1000);

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

void blueMogo() { // blue
    // mogo
    int sortState = 1;
    chassis.moveToPoint(0, -27, 2000, {.forwards = false, .maxSpeed = 50});
    chassis.waitUntilDone();
    mogoClamp.set_value(true);

    pros::delay(250);
    intakeUpper.move_voltage(12000);
    pros::delay(250);

    // // ring
    chassis.turnToHeading(93, 1000);
    chassis.moveToPoint(22, -27, 1000);
    pros::delay(2000);

    // drop mogo
    chassis.turnToHeading(216.7, 2000, {.direction = lemlib::AngularDirection::CW_CLOCKWISE});
    chassis.waitUntilDone();
    mogoClamp.set_value(false);

    // third mogo
    chassis.turnToHeading(-347.09, 1000);
    chassis.moveToPoint(21.7, -36.2, 1000, {.forwards = false});
    chassis.waitUntilDone();

    // // //ladder
    // chassis.turnToHeading(-256.4, 1000);
    // chassis.waitUntilDone();

    // mogoClamp.set_value(false);
    //  globalTarget = 180;
    //  chassis.moveToPoint(0, -40, 1000);

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