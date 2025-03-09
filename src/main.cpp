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
            std::vector<double> left = leftMotors.get_position_all();
            std::vector<double> right = rightMotors.get_position_all();
            pros::lcd::print(1, "LeftF Encoders: %f", left[0]);
            pros::lcd::print(2, "LeftM Encoders: %f", left[1]);
            pros::lcd::print(3, "LeftB Encoders: %f", left[2]);
            pros::lcd::print(4, "RightF Encoders: %f", right[0]);
            pros::lcd::print(5, "RightM Encoders: %f", right[1]);
            pros::lcd::print(6, "RightB Encoders: %f", right[2]);

            // pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            // pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            // pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // pros::lcd::print(3, "LB: %f", ladyBrown.get_position()); // lift encoder
            // //pros::lcd::print(0, "Color: %f ", optical.get_hue());
            // pros::lcd::print(5, "Intake Velocity: %f", intakeUpper.get_actual_velocity());

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

void fourRingRed() {
    chassis.setPose(0, 0, 0);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    sortState = 2;

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
    chassis.waitUntilDone();
    clampState = 0;

    //intakeState = 0;
    // chassis.waitUntilDone();
    // clampState = 0;

    chassis.moveToPoint(-1.52, -41.3, 6000); // go to the ladder

    globalTarget = 140;
}

void fourRingBlue() {
    chassis.setPose(0, 0, 0);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    sortState = 1;

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

    //intakeState = 0;
    chassis.waitUntilDone();
    clampState = 0;

    // face third mogo
    //chassis.turnToHeading(0, 500);
    //chassis.moveToPoint(21.71, -32, 1000);

    chassis.moveToPoint(-19, -34, 3000); //ladder

    // // globalTarget = 140;
}

void blueSoloWP() {
    //inversed blue
     sortState = 1;
    chassis.turnToHeading(28, 1000);
    chassis.moveToPoint(1.9, 2.7, 2000, {.forwards = false, .minSpeed = 70}); // move back to alliance
    chassis.waitUntilDone();

    //globalTarget = 140;
    pros::delay(600);
    //globalTarget = 0;

    chassis.moveToPoint(-8.8, -24.2, 2000,  {.forwards = false} ); //move to mogo 
    chassis.waitUntilDone();
    clampState = 1;
    pros::delay(250);
    chassis.turnToHeading(-147, 1000); //turn to face center stack

    chassis.moveToPose(-42, -48, -88.5, 2000); // go to center rings 
    intakeState = 1;
    pros::delay(250);

    chassis.moveToPose(-20.3, -43.3, -123, 2000, {.forwards = false}); //move back 

    chassis.turnToHeading(-66.2, 1000); //turn to face second ring stack

    chassis.moveToPoint(-32.25, -38.5, 1000); //move to second ring stack 
    pros::delay(1500);

    chassis.turnToHeading(90, 3000); // turn to face ladder
    chassis.waitUntilDone();



    chassis.moveToPoint(-9, -41, 5000); // go to ladder 
    globalTarget = 140;

       


}

void redSoloWP() {
    //inversed blue
     sortState = 2;
    //chassis.turnToHeading(-28, 1000);
    //chassis.moveToPoint(-1.9, 2.7, 2000, {.forwards = false, .minSpeed = 70}); //move back to alliance
    //chassis.waitUntilDone();

    //globalTarget = 140;
    pros::delay(600);
    globalTarget = 0;

    chassis.moveToPoint(7.36, -22, 2000,  {.forwards = false} ); //-23 //move to mogo
    chassis.waitUntilDone();
    clampState = 1;
    pros::delay(250);
    chassis.turnToHeading(147, 1000); //turn to face center stack

    chassis.moveToPose(37.2, -39, 90, 2000); //go to center rings
    intakeState = 1;

    chassis.moveToPose(13, -30.4, 123, 2000, {.forwards = false}); //move back

    chassis.turnToHeading(68.2, 1000); //turn to face second ring stack

    chassis.moveToPoint(22.5, -26.1, 1000); //25.5 //move to second ring stack

    chassis.turnToHeading(90, 3000); //turn to face ladder
      chassis.waitUntilDone();



    chassis.moveToPoint(-9, -24, 5000); //-3.7, -26.5 //go to ladder
         //globalTarget = 140;

       


}

//NOTE CHANGE globalTarget = 23 to globalTarget = 22
void skills() {

    sortState = 2;

    //alliance
    //chassis.moveToPoint(0, 1, 600); //make this more it doesnt really show 
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
    //globalTarget = 23;
    pros::delay(250);

    //back up and go to wall stake
    chassis.moveToPose(-36.1, 38.3, -51.6, 100000, {.forwards = false, .lead = 0.5, .minSpeed = 50});
    chassis.waitUntilDone();
    globalTarget = 22;
    // intakeState = 0;
    chassis.moveToPose(-53, 51.25, -86, 1000, {.lead = 0.7, .minSpeed = 50});
    pros::delay(250);
    // chassis.waitUntilDone();
    // intakeState = 2;
    // pros::delay(50);
    // globalTarget = 80;
    // intakeState = 1;
    chassis.moveToPose(-75, 51.25, -90, 1500);
    chassis.waitUntilDone();
    intakeState = 0;
    
    //score wall stake and get mogo ring #2
    intakeState = 2;
    pros::delay(50);
    globalTarget = 140;
    pros::delay(170);
    intakeState = 1;
    chassis.moveToPose(-75, 51.25, -90, 250);
    pros::delay(800);

    //back up and get mogo rings #3 4 5
    chassis.moveToPoint(-47, 49.5, 500, {.forwards = false});
    chassis.waitUntilDone();
      
    //globalTarget = 0;

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

    chassis.moveToPoint(-58, -0.5, 1500, {.forwards = false, .minSpeed = 50}); //move back into the corner


    chassis.moveToPoint(-54.5, 3.2, 1000);  // move out of the corner 


    //next mogo

    chassis.turnToHeading(-90, 1000); // turn to face mogo in the second corner 
    chassis.moveToPoint(0, 3.7, 4000, {.forwards = false}); // move to second mogo
    chassis.moveToPoint(6, 3.7, 400, {.forwards = false, .maxSpeed = 80}); // slow down for mogo
    chassis.waitUntilDone(); 
    clampState = 1; //clamp onto mogo 
    pros::delay(400);


    //mogo ring #1
    chassis.turnToHeading(0, 500); // turn to face ring 
    intakeState = 1;
    chassis.moveToPoint(4.5, 16, 500); // move to ring 
    pros::delay(250);


    //wall stake ring #1
    chassis.moveToPoint(18, 36, 800); // middle point to get the ring for wallstake 
    chassis.turnToHeading(25.4, 500); 
    chassis.moveToPoint(26, 58.3, 800); // wall stake ring point 
    chassis.waitUntilDone();
    //globalTarget = 23;
    pros::delay(250);

    //back up and go to wall stake
    chassis.moveToPose(20.7, 40.06, 35, 100000, {.forwards = false, .lead = 0.5, .minSpeed = 50});
    chassis.waitUntilDone();
    globalTarget = 22;
    //intakeState = 0;
    chassis.moveToPose(26.5, 46.7, 90, 100000, {.lead = 0.7, .minSpeed = 50});
    pros::delay(1250);
    // chassis.waitUntilDone();
    // intakeState = 2;
    // pros::delay(50);
    // globalTarget = 80;
    // intakeState = 1;
    chassis.moveToPoint(40.5, 44.5, 750);
    chassis.waitUntilDone();
    intakeState = 0;
    
    // //score wall stake and get mogo ring #2
    intakeState = 2;
    pros::delay(50);
    globalTarget = 140;
    pros::delay(170);
    intakeState = 1;
    chassis.moveToPoint(40.5, 44.5, 500);
    pros::delay(800);


    //back up and get mogo rings #3 4 5
    chassis.moveToPoint(25.5, 44.75, 500, {.forwards = false});
    chassis.waitUntilDone();
      
    //globalTarget = 0;

    
    chassis.turnToHeading(180, 500);
    chassis.moveToPose(27.12, -1.258, 180, 2500, {.maxSpeed = 80});
    chassis.waitUntilDone();

   
    pros::delay(750);

    chassis.turnToHeading(68.2, 1000); // getting third ring in the corner 

    chassis.moveToPoint(35.9, 3.9, 1000);

    chassis.turnToHeading(-31.3, 1000, {.maxSpeed = 90}); // turn to face the corner // TODO: might break some stuf
    chassis.waitUntilDone(); 
    intakeState = 2;
    pros::delay(25);
    intakeState = 1;

     pros::delay(300);

    clampState = 0;

    chassis.moveToPoint(35.5, 2, 2000, {.forwards = false, .minSpeed = 50});

    chassis.moveToPoint(31.3, 8.7, 1000); 



    //third quadrant

    //move to ring #1 on second half
    intakeKeep = true;
    chassis.turnToHeading(0, 500);
    chassis.moveToPose(2, 70.4, -61, 3000); // goes to ring on 3rd quadrant 
    //chassis.waitUntilDone();
    // intakeState = 1;
    // pros::delay(750);
    // intakeState = 0;

    //clamp to push
    chassis.turnToHeading(180, 500);
    chassis.moveToPoint(5.4, 91.9, 1000, {.forwards = false});
    chassis.waitUntilDone();
    intakeKeep = false;
    clampState = 1;
    pros::delay(500);

    //turn to corner
    chassis.turnToHeading(-109.8, 500);
    chassis.waitUntilDone();
    clampState = 0;

    chassis.moveToPoint(30.5, 100.3, 2000, {.forwards = false, .minSpeed = 100}); //push it into the corner 
    chassis.moveToPoint(6.7, 96.2, 1000);

    //turn to face fourth mogo 
    chassis.turnToHeading(-296, 1000);

    //grab mogo
    chassis.moveToPoint(-16.8, 85.1, 1000, {.forwards = false});
    chassis.waitUntilDone();

    clampState = 1;
    intakeState = 2;
    pros::delay(25);

    intakeState = 1;

    //move to fourth quadrant rings
    //chassis.turnToHeading(61.4, 500);
    chassis.moveToPoint(-52.7, 72.4, 1000, {.forwards = false, .maxSpeed = 100});
    chassis.waitUntilDone();

    // //drive through fourth quadrant rings
    chassis.turnToHeading(-395, 1000);
    chassis.moveToPoint(-58, 80.7, 500);
    pros::delay(500);
    chassis.turnToHeading(-442, 500);
    chassis.moveToPoint(-70.3, 80.5, 500);
    pros::delay(1000);
    chassis.moveToPoint(-58, 80.7, 500, {.forwards = false});
    pros::delay(250);
    chassis.turnToHeading(0, 500);
    chassis.moveToPoint(-61.2, 88.8, 1000);
    pros::delay(1500);


    //mogo
    chassis.moveToPoint(-58, 75.4, 500, {.forwards = false});
    chassis.turnToHeading(125.8, 500);
    chassis.waitUntilDone();
    intakeState = 2;
    pros::delay(50);
    intakeState = 0;
    pros::delay(300);
    clampState = 0;
    chassis.moveToPoint(-85.1, 101.2, 500, {.forwards = false, .minSpeed = 80});
    //pros::delay(3000);

    //hang
    chassis.moveToPoint(-54.1, 75.6, 500);
    chassis.turnToHeading(-45, 500);
    chassis.moveToPoint(-15.8, 31.9, 10000, {.forwards = false, .maxSpeed = 70});


    //no time :(

    // //alliance ring
    // chassis.moveToPoint(-56.2, 87.4, 500);
    // chassis.turnToHeading(146.6, 500);
    // chassis.moveToPoint(-42.4, 63.8, 750);
    // chassis.waitUntilDone();
    // intakeState = 1;
    // pros::delay(500);
    // intakeState = 0;

    // //go to alliance
    // chassis.turnToHeading(231.6, 500);
    // chassis.moveToPose(-21, 93, 180, 1500, {.forwards = false});
    // chassis.moveToPoint(-21, 100, 500, {.forwards = false});
    // chassis.waitUntilDone();

    // //score alliance
    // intakeState = 1;
    // pros::delay(1000);
    // intakeState = 0;
    // pros::delay(3000);

    // //go to ladder
    // chassis.moveToPose(-1.1, 61.2, 135, 2000);
    // chassis.turnToHeading(45, 500);
    // globalTarget = 140;
    // chassis.moveToPoint(-14.2, 52, 5000, {.forwards = false, .minSpeed = 50});
    // chassis.waitUntilDone();
    // //globalTarget = 0;

}

void autonomous() {
    chassis.setBrakeMode(MOTOR_BRAKE_BRAKE);
    //blueMogoSide();

    // sortState = 2;

    //fourRingBlue();
    //fourRingRed();
    //redSoloWP();
    //blueSoloWP();

    skills();

    
    // chassis.follow(ringsideRed_txt, ringsideExtra_txt, "ringside");
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
