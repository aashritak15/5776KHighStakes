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
            std::cout<<chassis.getPose().x<<", "<<chassis.getPose().y<<", "<<chassis.getPose().theta<<"\n";
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
            pros::lcd::print(5, "Intake Velocity: %f", intakeUpper.get_actual_velocity());

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
    chassis.moveToPoint(0, 1, 600); //make this more it doesnt really show 
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
    chassis.moveToPoint(-24, 36, 500);
    pros::delay(250);

    //wall stake ring #1
    chassis.turnToHeading(-30.5, 500);
    chassis.waitUntilDone();
    pros::delay(250);
    chassis.moveToPoint(-46, 70.9, 1000);
    chassis.waitUntilDone();
    globalTarget = 24.5;
    pros::delay(500);

    //back up and go to wall stake
    chassis.moveToPose(-36.1, 37.8, -51.6, 100000, {.forwards = false, .lead = 0.5, .minSpeed = 50});
    chassis.moveToPose(-53, 51.25, -86, 100000, {.lead = 0.7, .minSpeed = 50});
    chassis.moveToPose(-75, 51.25, -90, 1500);
    chassis.waitUntilDone();
    
    //score wall stake and get mogo ring #2
    intakeState = 2;
    pros::delay(50);
    globalTarget = 140;
    pros::delay(170);
    intakeState = 1;
    chassis.moveToPose(-75, 51.25, -90, 250);
    pros::delay(800);

    //back up and get mogo rings #3 4 5
    chassis.moveToPoint(-49.5, 49.5, 500, {.forwards = false});
    chassis.waitUntilDone();
      
    globalTarget = 0;

    chassis.turnToHeading(-180, 500);
    chassis.moveToPose(-49.5, -1, 180, 2500, {.maxSpeed = 80});
    chassis.waitUntilDone();

   
    pros::delay(500);

    chassis.turnToHeading(-56, 1000); // turns to get ring # 6 

    chassis.moveToPoint(-54.65, 2.65, 1000);

     pros::delay(1500);

    chassis.turnToHeading(35, 1000); // turn to face the corner 
    chassis.waitUntilDone(); 
     intakeState = 2; //outtake for a little bit to not let intake get stuck 
    pros::delay(50);
    intakeState = 0;
  

    pros::delay(300);

    clampState = 0;

    chassis.moveToPoint(-58, -0.5, 3000, {.forwards = false, .minSpeed = 50}); //move back into the corner


    chassis.moveToPoint(-54.5, 3.2, 1000);  // move out of the corner 


    //next mogo

    chassis.turnToHeading(-90, 1000); // turn to face mogo in the second corner 

   // chassis.moveToPoint(25, 12.3, 3000, {.forwards = false});

   chassis.moveToPoint(0, 3.7, 4000, {.forwards = false}); // move to second mogo
   chassis.moveToPoint(4, 3.7, 400, {.forwards = false, .maxSpeed = 80}); // slow down for mogo
   chassis.waitUntilDone(); 
    clampState = 1; //clamp onto mogo 
    pros::delay(400);


    //mogo ring #1
    chassis.turnToHeading(0, 500); // turn to face ring 
    intakeState = 1;
    chassis.moveToPoint(4.5, 16, 500); // move to ring 
    pros::delay(250);


    //wall stake ring #1
    chassis.moveToPoint(19, 36, 800); // middle point to get the ring for wallstake 
    chassis.turnToHeading(25.4, 500); 
    chassis.moveToPoint(27, 58.3, 800); // wall stake ring point 
    chassis.waitUntilDone();
    globalTarget = 24.5;
    pros::delay(500);

    //back up and go to wall stake
    chassis.moveToPose(20.7, 40.5, 35, 100000, {.forwards = false, .lead = 0.5, .minSpeed = 50});
    chassis.moveToPose(26.5, 40.5, 90, 100000, {.lead = 0.7, .minSpeed = 50});
    chassis.moveToPose(56.5, 44.75, 90, 500, {.minSpeed= 10});
    chassis.waitUntilDone();
    
    // //score wall stake and get mogo ring #2
    intakeState = 2;
    pros::delay(50);
    globalTarget = 140;
    pros::delay(170);
    intakeState = 1;
    chassis.moveToPose(56.5, 45.5, 90, 500, {.minSpeed= 10});
    pros::delay(800);


    //back up and get mogo rings #3 4 5
    chassis.moveToPoint(25.5, 44.75, 500, {.forwards = false});
    chassis.waitUntilDone();
      
    globalTarget = 0;

    
    chassis.turnToHeading(180, 500);
    chassis.moveToPose(27.12, -1.258, 180, 2500, {.maxSpeed = 80});
    

   

    // chassis.waitUntilDone();
    // chassis.moveToPose(-48.5, 13.14, 180, 500);
    // chassis.waitUntilDone();
    chassis.waitUntilDone();

   
    pros::delay(500);

    chassis.turnToHeading(68.2, 1000);

    chassis.moveToPoint(35.9, 3.9, 1000);

    chassis.turnToHeading(-31.3, 1000);
    chassis.waitUntilDone(); 
     intakeState = 2;
    pros::delay(50);
    intakeState = 1;

     pros::delay(300);

    clampState = 0;

    chassis.moveToPoint(35.5, 1, 3000, {.forwards = false, .minSpeed = 50});


    // chassis.moveToPoint(-54.5, 4.2, 1000); 

    //third quadrant

    //move to ring #1 on second half
    //intakeKeep = true;
    chassis.moveToPoint(31.1, 7.6, 500);
    chassis.turnToHeading(0, 500);
    chassis.moveToPose(3, 70.4, -59, 3000);
    chassis.waitUntilDone();
    //pros::delay(2000);
    intakeState = 0;

    //clamp to push
    chassis.turnToHeading(180, 500);
    chassis.moveToPoint(6, 87.23, 1000, {.forwards = false, .maxSpeed = 80});
    chassis.waitUntilDone();
    clampState = 1;
    pros::delay(500);
    //intakeKeep = false;

    //go to corner
    chassis.turnToHeading(-100, 500);
    chassis.waitUntilDone();
    clampState = 0;
    // pros::delay(200);
    chassis.moveToPoint(31.6, 94, 1000, {.forwards = false, .minSpeed = 100});

    //chassis.turnToHeading(-285, 500);
    //chassis.moveToPoint();

    
    //get other mogo
    chassis.moveToPoint(4.8, 84.8, 1000);
    chassis.turnToHeading(-280, 500);
    chassis.moveToPoint(-4.5, 83.2, 500, {.forwards = false});
    chassis.waitUntilDone();
    clampState = 1;
    pros::delay(600);

    // go to rings
    //ring 1
    chassis.turnToHeading(-115, 5000);
    chassis.waitUntilDone();
    intakeState = 1;
    pros::delay(1000);
    chassis.moveToPoint(-28, 74.5, 5000);
    pros::delay(1000);
    chassis.turnToHeading(-70, 5000);
    pros::delay(1000);
    chassis.moveToPoint(-57, 82.5, 5000);
    pros::delay(2000);

    //other rings
    chassis.moveToPoint(-46, 80.7, 5000, {.forwards = false});
    pros::delay(1000);
    chassis.turnToHeading(-21, 5000);
    pros::delay(1000);
    chassis.moveToPoint(-49.4, 89.1, 5000);
    pros::delay(2000);


    //mogo in corner
    chassis.turnToHeading(120, 7500);
    chassis.waitUntilDone();
    intakeState = 2;
    pros::delay(50);
    intakeState = 0;
    pros::delay(300);
    clampState = 0;
    chassis.moveToPoint(-64.2, 69.5, 5000, {.forwards = false});
    pros::delay(2000);

    // //alliance ring
    // chassis.moveToPoint(-43.8, 70.7, 500);
    // chassis.waitUntilDone();
    // intakeState = 1;
    // pros::delay(100);
    // intakeState = 0;

    // //go to alliance
    // chassis.turnToHeading(232, 500);
    // chassis.moveToPoint(-16.7, 93, 500);
    // chassis.turnToHeading(180, 500);
    // chassis.moveToPoint(-16.3, 99, 500);
    // chassis.waitUntilDone();
    // intakeState = 1;
    // pros::delay(750);
    // intakeState = 0;

    // //ladder
    // chassis.moveToPoint(-16.3, 92, 500);
    // chassis.turnToHeading(138, 500);
    // chassis.moveToPoint(5.6, 68.2, 500);
    // chassis.turnToHeading(45, 500);
    // chassis.waitUntilDone();
    // globalTarget = 140;
    // //chassis.moveToPoint()


}

void autonomous() {
    chassis.setBrakeMode(MOTOR_BRAKE_BRAKE);
    sortState = 0;
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
