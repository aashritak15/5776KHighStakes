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

    sortState = 2;

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

    chassis.turnToHeading(68.2, 1000); // getting third ring in the corner 

    chassis.moveToPoint(35.9, 3.9, 1000);

    chassis.turnToHeading(-31.3, 1000, {.maxSpeed = 90}); // turn to face the corner // TODO: might break some stuf
    chassis.waitUntilDone(); 
     intakeState = 2;
    pros::delay(50);
    intakeState = 1;

     pros::delay(300);

    clampState = 0;

    chassis.moveToPoint(35.5, 1, 3000, {.forwards = false, .minSpeed = 50});

    chassis.moveToPoint(31.3, 8.7, 1000); 

    //third quadrant

    //move to ring #1 on second half
    intakeKeep = true;
    chassis.turnToHeading(0, 500);
    chassis.moveToPose(3, 70.4, -59, 3000); // goes to ring on 3rd quadrant 
    chassis.waitUntilDone();

    //clamp to push
    chassis.turnToHeading(180, 500);
    chassis.moveToPoint(2.4, 92.9, 1000, {.forwards = false});
    chassis.waitUntilDone();
    clampState = 1;
    pros::delay(500);

    //turn to corner
    chassis.turnToHeading(-109.8, 500);
    chassis.waitUntilDone();
    clampState = 0;

    chassis.moveToPoint(30.5, 108.3, 3000, {.forwards = false, .minSpeed = 60}); //push it into the corner 
    chassis.moveToPoint(6.7, 96.2, 1000);

    //turn to face fourth mogo 
    chassis.turnToHeading(81.4, 1000);

    //grab mogo
    chassis.moveToPoint(-20.9, 90.5, 1000, {.forwards = false});
    chassis.waitUntilDone();

    clampState = 1;
    intakeState = 2;
    pros::delay(80);

    intakeKeep = false;
    intakeState = 1;

    //move to fourth quadrant rings
    chassis.turnToHeading(61.4, 500);
    // chassis.moveToPoint(-58.7, 71.9, 1000, {.forwards = false, .maxSpeed = 100});
    // chassis.waitUntilDone();

    // //drive through fourth quadrant rings
    // chassis.turnToHeading(0, 1000);
    // chassis.moveToPoint(-58.7, 101.4, 3000, {.maxSpeed = 80});

    // chassis.turnToHeading(-294, 1000); //if we are runnig out of time you can change the turntoHeading before this to this angle itself

    // chassis.moveToPoint(-49.9, 76.1, 1000); // move back into 4th quadrant

    // chassis.turnToHeading(0, 1000); //turn to face the rings 

 

    // // chassis.moveToPoint(-55.1, 71.9, 3000, {.forwards = false});

    // // chassis.turnToHeading(0, 800); 










}

void autonomous() {
    chassis.setBrakeMode(MOTOR_BRAKE_BRAKE);
    sortState = 0;
    skills();

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
