#include "globals.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "liblvgl/llemu.hpp"
#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "magic.hpp"
#include <cmath>
#include "ladybrown.hpp"

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::MotorGroup leftMotors({-8, -4, -12}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({18, 15, 11}, pros::MotorGearset::blue);

pros::Imu imu(3);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, 12.5, lemlib::Omniwheel::NEW_275, 450,
                              8 // 13.4 trackwidth artificial
);

// lateral motion controller
lemlib::ControllerSettings linearController(7.3, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            1, // 48.905, //46.86273, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            2, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            0 // slew
);

// angular motion controller
lemlib::ControllerSettings angularController(3.3, // proportional gain (kP) 4.6 //TODO: maybe tune more!
                                             0, // integral gain (kI)
                                             27, // 38,//37.88, // 32.92, //40.5, // derivative gain (kD) 2
                                             0, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // slew

                                             // OLD VALUES OCT 6: P 4.05, D 34.86768
);
// sensors for odometry
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null //TODO: what the actual flip
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// tanish driver functions:

lemlib::ExpoDriveCurve throttleCurve(5, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(5, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

ASSET(redMogoAlliancePath_txt);
ASSET(redMogoAllianceExtra_txt);
ASSET(autonomous_txt); 
ASSET(extra_txt);
ASSET(blueMogoAlliancePath_txt);
ASSET(skillsPath_txt); 
ASSET(skillsExtra_txt);

void screenTask() {
    while (true) {
        //* line 7 reserved for rerun states.
        pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
        pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
        pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
        pros::lcd::print(3, "Color: %f", optical.get_hue());
        pros::lcd::print(4, "LB Rot Sensor: %i", lbRotation.get_position());
        pros::lcd::print(5, "Intake vel: %lf", intakeLower.get_actual_velocity());
        pros::Task::delay(50);
    }
}

// void autonSelector() {
//     while (!colorSelected) {
//         if (pros::lcd::read_buttons() & LCD_BTN_LEFT) {
//             pros::lcd::print(7, "Left button");
//         } else if (pros::lcd::read_buttons() & LCD_BTN_CENTER) {
//             pros::lcd::print(7, "Center button");
//         } else if (pros::lcd::read_buttons() & LCD_BTN_RIGHT) {
//             pros::lcd::print(7, "Right button");
//         } else {
//             pros::lcd::print(7, "No button");
//         }

//         if (color == 0) {
//             pros::lcd::print(0, "Red");
//         } else if (color == 1) {
//             pros::lcd::print(0, "Blue");
//         }

//         if (pros::lcd::read_buttons() & LCD_BTN_CENTER) { // center button
//             if (!middlePressed) {
//                 middlePressed = true;
//                 if (color == 0 || color == 2) {
//                     color = 1;
//                 } else if (color == 1) {
//                     color = 0;
//                 }
//             }
//         } else {
//             middlePressed = false;
//         }

//         if (pros::lcd::read_buttons() & LCD_BTN_RIGHT) { // right
//             colorSelected = true;
//             pros::lcd::print(1, "Color Selected!");
//         }

//         pros::delay(100);
//     }

//     while (!autonSelected) {
//         if (pros::lcd::read_buttons() & LCD_BTN_RIGHT) { // right
//             if (auton == MAX) { auton = 1; }
//             auton++;
//         }

//         if (pros::lcd::read_buttons() & LCD_BTN_LEFT) { // left
//             if (auton == 1) { auton = MAX; }
//             auton--;
//         }

//         if (pros::lcd::read_buttons() & LCD_BTN_CENTER) { // center
//             autonSelected = true;
//             pros::lcd::print(3, "Auton Selected!");
//         }

//         switch (auton) {
//             case 1: pros::lcd::print(2, "Solo WP"); break;
//             case 2: pros::lcd::print(2, "Mogo Rush"); break;
//             case 3: pros::lcd::print(2, "Straight"); break;
//             case 4: pros::lcd::print(2, "Turn 90"); break;
//             case 5: pros::lcd::print(2, "Turn 180"); break;
//             case 6: pros::lcd::print(2, "Solo WP"); break;
//             case 7: pros::lcd::print(2, "Solo WP"); break;
//             case 8: pros::lcd::print(2, "Solo WP"); break;
//         }
//         pros::delay(100);
//     }

// std::cout<<"halsidjflas\n";

// pros::delay(250);
// pros::lcd::clear();
// pros::delay(250);
// pros::Task callScreenTask(screenTask, "screen task");
//}

/*
void autonSelector() {
    bool colorSelected = false;
    bool autonSelected = false;
    bool middlePressed = false;
    int color = 0;
    int auton = 1;
    const int MAX = 8;

    while (!colorSelected) {
        int buttons = pros::lcd::read_buttons();

        if (buttons & LCD_BTN_LEFT) {
            pros::lcd::print(7, "Left button");
        } else if (buttons & LCD_BTN_CENTER) {
            pros::lcd::print(7, "Center button");
        } else if (buttons & LCD_BTN_RIGHT) {
            pros::lcd::print(7, "Right button");
        } else {
            pros::lcd::print(7, "No button");
        }

        pros::lcd::clear_line(0);
        pros::lcd::print(0, color == 0 ? "Red" : "Blue");

        if (buttons & LCD_BTN_CENTER) {
            if (!middlePressed) {
                middlePressed = true;
                color = (color == 0 || color == 2) ? 1 : 0;
            }
        } else {
            middlePressed = false;
        }

        if (buttons & LCD_BTN_RIGHT) {
            colorSelected = true;
            pros::lcd::print(1, "Color Selected!");
        }

        pros::delay(100);
    }

    while (!autonSelected) {
        int buttons = pros::lcd::read_buttons();

        if (buttons & LCD_BTN_RIGHT) {
            auton = (auton == MAX) ? 1 : auton + 1;
        }

        if (buttons & LCD_BTN_LEFT) {
            auton = (auton == 1) ? MAX : auton - 1;
        }

        if (buttons & LCD_BTN_CENTER) {
            autonSelected = true;
            pros::lcd::print(3, "Auton Selected!");
        }

        pros::lcd::clear_line(2);
        switch (auton) {
            case 1: pros::lcd::print(2, "Solo WP"); break;
            case 2: pros::lcd::print(2, "Mogo Rush"); break;
            case 3: pros::lcd::print(2, "Straight"); break;
            case 4: pros::lcd::print(2, "Turn 90"); break;
            case 5: pros::lcd::print(2, "Turn 180"); break;
            case 6: pros::lcd::print(2, "Custom 1"); break;
            case 7: pros::lcd::print(2, "Custom 2"); break;
            case 8: pros::lcd::print(2, "Custom 3"); break;
        }

        pros::delay(100);
    }
}
*/