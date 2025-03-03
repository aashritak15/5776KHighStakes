#include "autons.hpp"

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

// void fourRingBlue() {
//     chassis.setPose(0, 0, 0);
//     chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

//     chassis.moveToPoint(-0.2, -29, 2000, {.forwards = false, .maxSpeed = 60}); // go to mogo

//     chassis.waitUntilDone();
//     clampState = 1;
//     pros::delay(250);

//     chassis.turnToHeading(90, 1000);

//     intakeState = 1;

//     pros::delay(250);

//     chassis.moveToPoint(22.4, -31, 2000); // intake ring # 1

//     chassis.turnToHeading(0, 1000);

//     chassis.moveToPoint(23, -14, 1000);

//     pros::delay(500);

//     chassis.turnToHeading(44, 1000); // turn to face ring stack

//     intakeState = 2;

//     chassis.moveToPoint(37.2, 0.9, 700);
//     chassis.waitUntilDone(); // go to ring stack

//     intakeState = 1;

//     pros::delay(500);
//     chassis.moveToPoint(28.7, -4, 1000, {.forwards = false}); // go back

//     chassis.turnToHeading(266, 1000); // turn to face ladder

//     chassis.moveToPoint(1.23, -43, 6000); // go to the ladder
// }

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