// #include "main.h"
// #include "lemlib/api.hpp"
// #include "ports.hpp"
// #include "piston.hpp"
// #include "intake.hpp"
// #include "globals.hpp"
// #include "lift.hpp"
// #include "intakeFirst.hpp"
// #include "intakePiston.hpp"

// void redSoloWP() {
//     double liftPosition = lift.get_position();

//     // theta = 32.26
//     // go to mogo and clamp
//     mogoClamp.set_value(true);
//     chassis.moveToPoint(0, -16, 2000, {.forwards = false});
//     chassis.moveToPoint(0, -21, 3000, {.forwards = false, .maxSpeed = 50});
//     chassis.waitUntilDone();

//     mogoClamp.set_value(false);
//     intake.move_voltage(-12000);
//     pros::delay(150);

//     // go to first ring
//     // chassis.turnToHeading(78.2, 800);
//     // chassis.moveToPose(11.04, -18.63, 82.26, 2000);
//     chassis.moveToPose(11.29, -17.44, 76.95, 2000);
//     intakeFirst.move_velocity(-600);

//     // pros::delay(250;0); //could be less IF  ERROR UNCOMMENT THIS
//     //  LINE <---------------------

//     //second ring
//     chassis.turnToHeading(143, 1000);
//     chassis.moveToPoint(20.8, -26.01, 3000);
    

//     chassis.waitUntilDone(); // could be less

//     // go to point and release mogo
//     pros::delay(250);
//     intake.move_voltage(0);
//     chassis.moveToPoint(-5.83, -3.54, 2000, {.forwards = false});
//     chassis.waitUntilDone();
//     chassis.turnToHeading(249, 800);

//     chassis.waitUntilDone();
//     mogoClamp.set_value(true);

//     chassis.waitUntilDone();
//     pros::delay(500);

//     // move to alliance (push rings) and turn to align with alliance
//     intakeFirst.move_velocity(0);
//     // chassis.moveToPose(-24.5, -15.7, 243.33, 3000, {.earlyExitRange = 5});

//     chassis.moveToPose(-29.4, -11.6, 244.33, 3000, {.earlyExitRange = 5});
    
//     chassis.turnToHeading(153, 1000);
//     mogoClamp.set_value(false);

//     // move backwards to alliance stake
//     chassis.moveToPose(-31.4, -4.2, 153, 2000, {.forwards = false});
//     chassis.waitUntilDone();
    
//     intake.move_voltage(-12000);

//     // score on alliance stake with lift
//     lift.move_absolute(183, 40);
//     pros::delay(1500);
//     intake.move_voltage(0);
//     lift.move_absolute(liftPosition, -60);

//     // go to ladder
//     intakeFirst.move_voltage(12000);
//     chassis.moveToPose(-20, -30.1, 161, 3000, {.maxSpeed = 50});
// }

// void redMogo() {
//     double liftPosition = lift.get_position();

//     mogoClamp.set_value(true);

//     // mogo rush
//     chassis.moveToPoint(0, -28, 1000, {.forwards = false});
//     chassis.moveToPose(6.3, -44.4, -29.27, 2000, {.forwards = false, .maxSpeed = 50});

//     chassis.waitUntilDone();

//     mogoClamp.set_value(false);
//     chassis.turnToHeading(16.4, 800);
//     pros::delay(100);
//     intake.move_voltage(-12000);
//     pros::delay(800);
    
//     //chassis.moveToPoint(6.98, -38.98, 1000);

//     intakeFirst.move_velocity(-600);

//     chassis.moveToPoint(8, -36.5, 1000);

//     pros::delay(450);

//     //intakeFirst.move_velocity(600);

//     chassis.turnToHeading(124.38, 1000);

//     chassis.waitUntilDone();

//     pros::delay(600);

//     chassis.turnToHeading(99.1, 500);
//     //chassis.moveToPoint(22.8, -35.57, 3000, {.maxSpeed = 30});
//     chassis.moveToPoint(24.3, -37.5, 3000, {.maxSpeed = 30});

//     intakeFirst.move_velocity(0);
//     //mogoClamp.set_value(true);
// }

// void blueSoloWP() {
//     double liftPosition = lift.get_position();

//     // theta = 32.26

//     // go to mogo
//     mogoClamp.set_value(true);
//     chassis.moveToPoint(0, -16, 2000, {.forwards = false});
//     chassis.moveToPoint(0, -21, 3000, {.forwards = false, .maxSpeed = 50});
//     chassis.waitUntilDone();

//     // clamp
//     mogoClamp.set_value(false);
//     intake.move_voltage(-12000);
//     pros::delay(150);

//     // go to first ring
//     chassis.moveToPose(-11.29, -17.44, -80.95, 2000);
//     intakeFirst.move_velocity(-600);
//     pros::delay(250);

//     // go away from first ring
//     chassis.moveToPose(-16.3, -31.6, -142.89, 2000);
//     chassis.waitUntilDone(); // could be less

//     // release mogo
//     pros::delay(250);
//     intake.move_voltage(0);
//     chassis.moveToPoint(13.2, -10.96, 2000, {.forwards = false, .maxSpeed = 100});
//     chassis.waitUntilDone();
//     chassis.turnToHeading(-238.5, 800);
//     chassis.waitUntilDone();
//     mogoClamp.set_value(true);
//     chassis.waitUntilDone();
//     pros::delay(500);
//     intakeFirst.move_velocity(0);
//     // chassis.moveToPose(-24.5, -15.7, 243.33, 3000, {.earlyExitRange = 5});

//     // move to alliance stake
//     // chassis.moveToPose(39.3, -14.4, -240.5, 3000);
//     chassis.moveToPose(34.77, -15.74, -244, 3000);
//     //chassis.turnToHeading(207.34, 1000, {.direction = AngularDirection::CW_CLOCKWISE});
//     chassis.turnToPoint(42.6, -7.6, 1000, {.forwards = false});
//     mogoClamp.set_value(false);

//     // score alliance stake

//     chassis.moveToPoint(42.6, -7.6, 2000, {.forwards = false});
//     chassis.waitUntilDone();
//     intake.move_voltage(-12000);
//     lift.move_absolute(183, 40);
//     pros::delay(1500);
//     intake.move_voltage(0);
//     lift.move_absolute(liftPosition, -60);

//     // move to ladder

//     intakeFirst.move_voltage(12000); // outtake to repel rings in the way of ladder
//     chassis.moveToPose(28.1, -31.9, 220.6, 3000, {.maxSpeed = 38});
// }

// void blueMogo() {
//     double liftPosition = lift.get_position();

//     mogoClamp.set_value(true);

//     // mogo rush
//     chassis.moveToPoint(0, -30, 3000, {.forwards = false});

    
    
//     chassis.moveToPose(-5, -43.9, 30, 2000, {.forwards = false, .maxSpeed = 50});

//     chassis.waitUntilDone();

//     mogoClamp.set_value(false);
//     chassis.turnToHeading(-16.4, 800);
//     pros::delay(100);
//     intake.move_voltage(-12000);
//     pros::delay(800);
    
//     //chassis.moveToPoint(6.98, -38.98, 1000);

//     intakeFirst.move_velocity(-600);

//     chassis.moveToPoint(-8, -36.5, 1000);

//     pros::delay(450);

//     //intakeFirst.move_velocity(600);

//     // chassis.turnToHeading(-124.38, 1000);

//     chassis.waitUntilDone();

//     pros::delay(600);

//     // chassis.moveToPoint(-27, -47, 3000, {.maxSpeed = 35});

//     intakeFirst.move_velocity(0);

//     //mogoClamp.set_value(true);
    

//     /*
//     double liftPosition = lift.get_position();

//     mogoClamp.set_value(true);

//     // mogo rush
//     chassis.moveToPoint(0, -28, 1000, {.forwards = false});
//     chassis.moveToPose(-5.47, -44.78, 34.22, 2000, {.forwards = false});

//     chassis.waitUntilDone();

//     mogoClamp.set_value(false);
//     pros::delay(100);
//     intake.move_voltage(-12000);
//     pros::delay(300);
//     chassis.turnToHeading(-21.03, 800);
//     chassis.moveToPoint(-6.98, -38.98, 1000);

//     intakeFirst.move_velocity(-600);

//     chassis.moveToPoint(-11.29, -27.50, 1000);

//     pros::delay(450);

//     intakeFirst.move_velocity(600);

//     chassis.turnToHeading(-124.38, 1000);

//     chassis.waitUntilDone();

//     pros::delay(600);

//     chassis.moveToPoint(-33.81, -46.4, 3000, {.maxSpeed = 45});

//     mogoClamp.set_value(true); */
// }

// void skills() {
//     double liftPosition = lift.get_position();

//     // score alliance
//     lift.move_absolute(183, 40);
//     intake.move_voltage(-12000);
//     pros::delay(750);

//     // ring 1
//     lift.move_absolute(liftPosition, -60);
//     intakeFirst.move_velocity(-600);
//     intake.move_voltage(0);
//     chassis.moveToPose(30.19, 43.45, 42.34, 2000); // chassis.moveToPose(24.2, 36.14, 43.47, 2000);
//     chassis.waitUntilDone();

//     // mogo

//     mogoClamp.set_value(true);

//     chassis.moveToPose(27.1, 23.4, 27.95, 2000, {.forwards = false, .maxSpeed = 50});

//     chassis.waitUntilDone();
//     mogoClamp.set_value(false);

//     pros::delay(50);
//     intake.move_voltage(-12000);

//     // rings 2, 3, 4
//     chassis.moveToPose(44.4, 37.1, 50.97, 2000); // theta: 416.28
//     pros::delay(1550);
//     chassis.turnToHeading(180, 800);
//     chassis.moveToPoint(49.23, 12.5, 4000, {.maxSpeed = 30});
//     chassis.waitUntilDone();

//     // ring 5
//     chassis.moveToPoint(46.66, 33.87, 1000, {.forwards = false, .maxSpeed = 70});
//     pros::delay(1000);
//     chassis.turnToHeading(128.8, 800);
//     chassis.moveToPoint(54.38, 26.94, 1000);
//     pros::delay(2500);

//     // mogo to corner
//     chassis.turnToHeading(-18.57, 800, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});

//     chassis.moveToPoint(59.24, 13.64, 1000, {.forwards = false});
//     chassis.waitUntilDone();
//     pros::delay(300);
//     mogoClamp.set_value(true);

//     // position forward
//     chassis.moveToPoint(55, 23.8, 1000);
//     chassis.turnToHeading(90, 800);

//     // move to second mogo
//     chassis.moveToPose(
//         -10, 23.13, 90, 3000,
//         {.forwards = false,
//          .maxSpeed = 85}); // chassis.moveToPose(-11.19, 21.55, 90, 3000, {.forwards = false, .maxSpeed = 95});
//     chassis.moveToPose(
//         -15.93, 23.13, 90, 2000,
//         {.forwards = false,
//          .maxSpeed = 10}); // chassis.moveToPose(-16.6, 21.55, 90, 2000, {.forwards = false, .maxSpeed = 15});
//     chassis.waitUntilDone();
//     pros::delay(1000);

//     // clamp onto second mogo
//     mogoClamp.set_value(false);

//     chassis.moveToPoint(-23.6, 28.2, 3000, {.forwards = false});

//     // first ring on second side

//     intakeFirst.move_velocity(-600);
//     intake.move_voltage(-12000);

//     chassis.turnToHeading(0, 2000);
//     chassis.moveToPose(-23.6, 46.8, 0, 3000);

//     // 2nd ring on second side
//     pros::delay(300);
//     chassis.turnToHeading(-83, 2000);
//     chassis.moveToPoint(-45.5, 53, 3000);

//     pros::delay(1000);

//     // 3rd + 4th ring
//     chassis.turnToHeading(-176, 2000);
//     chassis.moveToPoint(-46.2, 18, 4000, {.maxSpeed = 25});
//     pros::delay(2000);

//     chassis.moveToPoint(-46.2, 35.2, 1000, {.forwards = false});
//     pros::delay(300);
//     chassis.turnToHeading(-130.3, 1000);

//     // 5th ring

//     chassis.moveToPoint(-52.4, 30.2, 3000, {.forwards = true});
//     chassis.waitUntilDone();
//     pros::delay(1300);

//     chassis.turnToHeading(29.4, 1000);
//     pros::delay(3000);

//     chassis.moveToPose(-62.7, 13.3, 29.4, 3000, {.forwards = false});

//     mogoClamp.set_value(true);
// }
