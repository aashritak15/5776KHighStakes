#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "ports.hpp"
#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "ladybrown.hpp"
#include "intakeFirst.hpp"
#include "autons.hpp"
#include "magic.hpp"
#include <iostream>
#include <cmath>

// #include "autoSelect/selection.h"

// rd::Selector selector({
//    {"Blue AWP", &auton0},
//    {"Red AWP", &simple_auton},
//    {"Blue Rush" , &auton1},
//    {"Red Rush", &auton2}
//    {"Skills Run", &skills}
// });

//pros::MotorGroup leftMotors({-3, -2, -16}, pros::MotorGearset::blue);
//pros::MotorGroup rightMotors({19, 20, 18}, pros::MotorGearset::blue);

pros::MotorGroup leftMotors({11, 12}, pros::MotorGearset::green);
pros::MotorGroup rightMotors({-14, -4}, pros::MotorGearset::green);

pros::Imu imu(19);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, 8.5, lemlib::Omniwheel::NEW_4, 200,
                              2 // 2 w/o traction
);

// lateral motion controller
lemlib::ControllerSettings linearController(12, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            4, // 48.905, //46.86273, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            2, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            0 // slew
);

// angular motion controller
lemlib::ControllerSettings angularController(1.5, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             7, // 38,//37.88, // 32.92, //40.5, // derivative gain (kD)
                                             0, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // slew

                                             // OLD VALUES OCT 6: P 4.05, D 34.86768
);
// sensors for odometry
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// tanish driver functions:

lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors

    clampInit();
    intakeInnit();
    liftInit();
    intakeClampInit();
    opticalInit();
    rotationInit();
    initO();

    // lv_init();
    // selector::init();

    // the default rate is 50. however, if you need to change the rate, you
    // can do the following.
    // lemlib::bufferedStdout().setRate(...);
    // If you use bluetooth or a wired connection, you will want to have a rate of 10ms

    // for more information on how the formatting for the loggers
    // works, refer to the fmtlib docs

    // thread to for brain screen and position logging

    pros::Task screenTask([&]() {
        while (true) {
            // print robot location to the brain screen
            
        //     std::vector<double> left = leftMotors.get_position_all();
        // std::vector<double> right = rightMotors.get_position_all();
        // pros::lcd::print(5, "LeftF Encoders: %f", left[0]);
        // pros::lcd::print(6, "LeftM Encoders: %f", left[1]);
        // //pros::lcd::print(7, "LeftB Encoders: %f", left[2]);
        // pros::lcd::print(2, "RightF Encoders: %f", right[0]);
        // pros::lcd::print(3, "RightM Encoders: %f", right[1]);
        //pros::lcd::print(4, "RightB Encoders: %f", right[2]);

            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            pros::lcd::print(3, "Rotation: %f", rotationSensor.get_position()); // lift encoder
            pros::lcd::print(4, "Encoder: %f", lift.get_position());
            //pros::lcd::print(4, "Color: %f", optical.get_hue());

            // log position telemetry
            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
            // delay to save resources
            pros::delay(50);
        }
    });
}


void writePose() {
   std::string dataLine = "";
   lemlib::Pose pose = chassis.getPose();
   std::uint32_t left = leftMotors.get_voltage();
   std::uint32_t right = rightMotors.get_voltage();   

//rounds to 3 decimal places (idk if that helps)
   dataLine.append(std::to_string((round(pose.x*1000))/1000) + ", ");
   dataLine.append(std::to_string((round(pose.y*1000))/1000) + ", ");
   dataLine.append(std::to_string((right+left)/2.0*127.0/12000.0) + "\n");

    std::cout<<std::to_string((round(pose.x*1000))/1000) + ", ";
    std::cout<<std::to_string((round(pose.y*1000))/1000) + ", ";
    std::cout<<std::to_string((right+left)/2.0*127.0/12000.0) + "\n";
//    dataLine.append(std::to_string(pose.x) + ", ");
//    dataLine.append(std::to_string(pose.y) + ", ");
//    dataLine.append(std::to_string(pose.theta) + "\n");


   fileO << dataLine;
        
//    fileO.flush();

   //writeAdditional();
}

void writeAdditional() {
    std::string dataLine = "";

    if(leftMotors.get_voltage() < 0 && rightMotors.get_voltage()<0)
        dataLine.append("1, ");
    else
        dataLine.append("0, ");

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) { //non toggle subsystem
        dataLine.append("1\n");
    }
    else
        dataLine.append("0\n");

    fileOTwo << dataLine;
    // fileOTwo.flush();
}

// void writeDebug() {

//     fileOTwo << lemlib::
//     // fileOTwo.flush();
// }


/**
 * Runs while the robot is disabled
 */
void disabled() {}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}

/**
 * Runs during auto
 *
 * This is an example autonomous routine which demonstrates a lot of the features LemLib has to offer
 */

ASSET(autonomous_txt);
ASSET(extra_txt);


void autonomous() {
    // const asset& path = autonomous_txt;
    // const std::string data(reinterpret_cast<char*>(path.buf), path.size);
    // if(data.size()!=0)
    //     controller.set_text(0, 0, "data");
    // else
    //     controller.set_text(0, 0, "no data");

   
    // chassis.moveToPoint(0, 24, 3000);
    // chassis.turnToHeading(180, 2500);

    // chassis.moveToPoint(0, 0, 2000);
    // chassis.turnToHeading(0, 2500);
    
    // chassis.moveToPoint(0, 24, 2000);
    // chassis.turnToHeading(180, 2500);

     //chassis.moveToPose(12, 24, 90, 5000, {.minSpeed = 20});
    
    std::cout<<"rerun\n";
    chassis.follow(autonomous_txt, extra_txt, 10, 40000); //TODO: use this later lol
    //std::cout<<"jerry\n";
    //chassis.follow(example_txt, extra1_txt, 10, 40000); //TODO: use this later lol

    
    // std::vector<std::vector<std::string>> subValues = getSubData(extra_txt);
    // pros::lcd::print(0, "pls: %f", subValues[1][0]);
    
    // with selector

    /*if (selector::auton == 1) { redSoloWP(); }
    if (selector::auton == 2) { blueSoloWP(); }
    if (selector::auton == 3) { redMogo(); }
    if (selector::auton == -1) { blueMogo(); }
    if (selector::auton == 0) { skills(); }*/

    // without selector

    // chassis.follow(path_jerryio_txt, 15, 3000);

    // chassis.turnToHeading(180, 10000);

    // chassis.moveToPoint(0, 24, 2000);
}

/**
 * Runs in driver control
 */

void opcontrol() {
    // controller
    // loop to continuously update motors
    initO();

    while(true) {
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        chassis.arcade(leftY, rightX);

        static unsigned long lastWriteTime = 0; // Tracks the last time writePose was called
        unsigned long currentTime = pros::millis(); // Get the current time in milliseconds

        if (currentTime - lastWriteTime >= 1000) {
            writePose();
            writeAdditional();
            lastWriteTime = currentTime; // Update the last write time
        }

        closeO();

        pros::delay(10);
    }


       updateIntake();
        updateIntakeFirst();
        updateClamp();
        updateIntakeClamp();
        updateLift();
        updateColorToggle();
        colorSort();
        updateDoinker();

 //TODO: WORKING LOOP!
    // chassis.follow(autonomous_txt, extra_txt, 1, 40000);
    // lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    // rotationSensor.reset_position();
    // while (true) {
        
        
    //     int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    //     int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

    //     chassis.arcade(leftY, rightX * 0.8);

        
        
    //     // pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
    //     // pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
    //     // pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
    //     // pros::lcd::print(3, "Rotation: %f", rotationSensor.get_position()); // lift encoder
    //     // pros::lcd::print(4, "Encoder: %f", lift.get_position());
    //     // //pros::lcd::print(4, "Color: %f", optical.get_hue());

    //     // lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());

 

    //     if (sortState == 0) {
    //         controller.set_text(0, 0, "no sort   ");
    //     } else if (sortState == 1) {
    //         controller.set_text(0, 0, "scores blue");
    //     } else if (sortState == 2) {
    //         controller.set_text(0, 0, "scores red ");
    //     }

    //     if (sortState == 0) {
    //         controller.set_text(0, 0, "no sort   ");
    //     } else if (sortState == 1) {
    //         controller.set_text(0, 0, "scores blue");
    //     } else if (sortState == 2) {
    //         controller.set_text(0, 0, "scores red ");
    //     }
        
    //     pros::delay(10); 
    // }
}
