#include "autons.hpp"

void fourRingRed() {
    chassis.setPose(0, 0, 0);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    chassis.moveToPoint(0.2, -28, 2000, {.forwards = false, .maxSpeed = 60}); // go to mogo

    chassis.waitUntilDone();
    mogoClamp.set_value(true);
    pros::delay(250);

    chassis.turnToHeading(-90, 1000);

    intakeUpper.move_velocity(600);
    intakeLower.move_velocity(-200);

    pros::delay(250);

    chassis.moveToPoint(-22.4, -28, 2000); // intake ring # 1

    chassis.turnToHeading(0, 1000);

    chassis.moveToPoint(-23, -11, 1000);

    pros::delay(500);

    chassis.turnToHeading(-43, 1000); // turn to face ring stack

    chassis.moveToPoint(-36, 2, 1000); // go to ring stack
    intakeUpper.move_velocity(-600);
    intakeLower.move_velocity(200);
    chassis.waitUntilDone();

    intakeUpper.move_velocity(600);
    intakeLower.move_velocity(-200);

    pros::delay(500);
    chassis.moveToPoint(-31.7, -1, 1000, {.forwards = false, .maxSpeed = 40}); // go back

    intakeUpper.move_velocity(600);
    intakeLower.move_velocity(-200);

    chassis.waitUntilDone();

    intakeUpper.move_velocity(-600);
    intakeLower.move_velocity(200);

    chassis.moveToPoint(-34, 0, 1000);

    intakeUpper.move_velocity(-600);
    intakeLower.move_velocity(200);
    chassis.waitUntilDone();

    intakeUpper.move_velocity(600);
    intakeLower.move_velocity(-200);

    pros::delay(500);
    chassis.moveToPoint(-28.7, -4, 1000, {.forwards = false, .maxSpeed = 40}); // go back

    intakeUpper.move_velocity(600);
    intakeLower.move_velocity(-200);

    chassis.waitUntilDone();

    intakeUpper.move_velocity(-600);
    intakeLower.move_velocity(200);

    chassis.moveToPoint(-34, 0, 1000); // go to ring stack
    intakeUpper.move_velocity(-600);
    intakeLower.move_velocity(200);
    chassis.waitUntilDone();

    intakeUpper.move_velocity(600);
    intakeLower.move_velocity(-200);

    pros::delay(500);
    chassis.moveToPoint(-26.7, -6, 1000, {.forwards = false, .maxSpeed = 40});
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