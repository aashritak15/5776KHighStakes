#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "ladybrown.hpp"
#include "magic.hpp"
#include <cmath>

void blueSoloWP() {
    chassis.moveToPose(3.4, 5.3, 33.3, 1000, {.maxSpeed = 30});
    chassis.waitUntilDone();
    pros::delay(100);
    ladyBrown.move_absolute(915, 100); // change to score angle
    pros::delay(500);
    ladyBrown.move_absolute(0, 100);
    pros::delay(100);

    // mogo
    chassis.turnToHeading(21.7, 1000);
    chassis.moveToPoint(-7.4, -24.9, 3000, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    mogoClamp.set_value(true);
    pros::delay(250);

    // center rings
    chassis.turnToHeading(-140.7, 2000);
    intake.move_voltage(-12000);
    chassis.moveToPoint(-24.5, -46.4, 2000);
    chassis.turnToHeading(-116, 1000);
    chassis.moveToPoint(-29.8, -48, 2000);
    chassis.waitUntilDone();
    pros::delay(1000);

    // middle ring
    chassis.moveToPoint(-10.6, -44.7, 2000, {.forwards = false});
    chassis.turnToHeading(-60, 1000);
    chassis.moveToPoint(-24.8, -34.7, 2000);
    pros::delay(1750);

    // //ladder
    // chassis.turnToHeading(-229.6, 1000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
    // chassis.moveToPoint(-12.7, -48.3, 2000, {.maxSpeed = 50});
    // chassis.waitUntilDone();
    // intake.move_voltage(0);

    // elims
    chassis.moveToPose(36, -15, 90, 2000);
    chassis.waitUntilDone();
    intake.move_voltage(0);
}

void redSoloWP() {
    // inversed blue
    chassis.moveToPose(-3.4, 5.3, -33.3, 1000, {.maxSpeed = 30});
    chassis.waitUntilDone();
    pros::delay(100);
    ladyBrown.move_absolute(915, 100); // change to score angle
    pros::delay(500);
    ladyBrown.move_absolute(0, 100);
    pros::delay(100);

    // mogo
    chassis.turnToHeading(-21.7, 1000);
    chassis.moveToPoint(7.4, -24.9, 3000, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    mogoClamp.set_value(true);
    pros::delay(250);

    // center rings
    chassis.turnToHeading(144.1, 2000);
    intake.move_voltage(-12000);
    chassis.moveToPoint(22.2, -49.3, 2000);
    pros::delay(500);
    chassis.turnToHeading(114.6, 1000);
    chassis.moveToPoint(28.5, -52.1, 2000);
    chassis.waitUntilDone();
    pros::delay(1000);

    // middle ring
    chassis.moveToPoint(10.6, -44.7, 2000, {.forwards = false});
    chassis.turnToHeading(60, 1000);
    chassis.moveToPoint(24.8, -34.7, 2000);
    pros::delay(1750);

    // //ladder
    // chassis.turnToHeading(229.6, 1000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
    // chassis.moveToPoint(12.7, -48.3, 2000, {.maxSpeed = 50});
    // chassis.waitUntilDone();
    // intake.move_voltage(0);

    // elims
    intake.move_voltage(0);
    chassis.moveToPose(-36, -15, -90, 2000);
    chassis.waitUntilDone();

    // old red
    //  ladyBrown.move_absolute(915, 100); // change to score angle
    //  pros::delay(500);
    //  ladyBrown.move_absolute(0, 100);
    //  pros::delay(100);

    // // mogo
    // // chassis.turnToHeading();
    // // chassis.moveToPoint(-4.7, -29.9, 3000, {.forwards = false, .maxSpeed = 80});
    // chassis.moveToPoint(-6.45, -34.1, 2000, {.forwards = false, .maxSpeed = 80});
    // chassis.waitUntilDone();
    // // pros::delay(100);
    // mogoClamp.set_value(true);
    // pros::delay(250);

    // // center rings
    // chassis.turnToHeading(166, 2000);
    // intake.move_voltage(-12000);

    // chassis.moveToPoint(-4.079, -61.039, 3000, {.forwards = true, .maxSpeed = 80});

    // chassis.turnToHeading(135, 2000);

    // chassis.moveToPoint(-0.7, -64.4, 3000, {.minSpeed = 80});
    // chassis.waitUntilDone();
    // pros::delay(1000);

    // // middle ring
    // //-9.8, -53.2, 146
    // chassis.moveToPoint(-16.2, -47.9, 2000, {.forwards = false}); // get new point in between so it doesnt cross line
    // chassis.turnToHeading(98, 1000);
    // chassis.moveToPoint(2.2, -46.4, 2000);
    // pros::delay(2000);

    // //ladder
    // chassis.turnToHeading(259.6, 1000);
    // chassis.moveToPoint(-20, -50.5, 2000);
    // chassis.waitUntilDone();
    // intake.move_voltage(0);
}

void redMogo() {
    // mogo
    chassis.moveToPoint(0, -30, 2000, {.forwards = false});
    chassis.waitUntilDone();
    mogoClamp.set_value(true);
    pros::delay(250);
    intake.move_voltage(-12000);
    pros::delay(250);

    // ring
    chassis.turnToHeading(-100, 1000);
    chassis.moveToPoint(-17.7, -35, 1000);
    pros::delay(250);

    // //ladder
    // chassis.turnToHeading(-236.4, 1000);
    // chassis.moveToPoint(-0.2, -45.8, 1000);
    // intake.move_voltage(0);

    // elims
    chassis.turnToHeading(-233.7, 2000);
    chassis.moveToPoint(-37, -19.6, 2000, {.forwards = false});
    chassis.waitUntilDone();
    mogoClamp.set_value(false);
    intake.move_voltage(0);
    chassis.moveToPoint(-24.5, -33, 2000);
    chassis.turnToHeading(7, 2000);
}

void blueMogo() {
    // mogo
    chassis.moveToPoint(0, -30, 2000, {.forwards = false});
    chassis.waitUntilDone();
    mogoClamp.set_value(true);
    pros::delay(250);
    intake.move_voltage(-12000);
    pros::delay(500);

    // ring
    chassis.turnToHeading(100, 1000);
    chassis.moveToPoint(17.7, -35, 1000);
    pros::delay(2000);

    // ladder
    chassis.turnToHeading(236.4, 1000);
    chassis.waitUntilDone();
    intake.move_voltage(0);
    mogoClamp.set_value(false);
    // chassis.moveToPoint(-0.8, -45.8, 1000);
    chassis.moveToPoint(-5.4, -44.6, 1000, {.maxSpeed = 50}); // 238.7
}