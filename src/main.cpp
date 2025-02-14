#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "ladybrown.hpp"
#include "magic.hpp"
#include "auton.hpp"
#include <cmath>

void initialize() {
    pros::lcd::initialize();

    clampInit();
    doinkInit();
    intakeInit();
    ladyBrownInit();

    chassis.calibrate(true);
    chassis.setPose(0, 0, 0);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    pros::Task screenTask([&]() {
        while (true) {
            //* line 7 reserved for rerun states.
            // std::cout<<chassis.getPose().x<<", "<<chassis.getPose().y<<", "<<chassis.getPose().theta<<"\n";

            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            pros::lcd::print(3, "Color: %f", optical.get_hue());
            pros::lcd::print(4, "LB Rot Sensor: %i", lbRotation.get_position());
            pros::lcd::print(5, "Drive vel: %i", leftMotors.get_voltage());
            // pros::lcd::print(5, "Tracking: %i", horizontalEnc.get_position());

            // pros::lcd::print(3, "Rotation (Lift): %i", rotationSensor.get_position()); // lift encoder
            // pros::lcd::print(4, "Intake State: %f", intakeState);

            // std::vector<double> left = leftMotors.get_position_all();
            // std::vector<double> right = rightMotors.get_position_all();
            // pros::lcd::print(3, "LeftF Encoders: %f", left[0]);
            // pros::lcd::print(4, "LeftM Encoders: %f", left[1]);
            // pros::lcd::print(5, "LeftB Encoders: %f", left[2]);
            // pros::lcd::print(6, "RightF Encoders: %f", right[0]);
            // pros::lcd::print(7, "RightM Encoders: %f", right[1]);
            // pros::lcd::print(8, "RightB Encoders: %f", right[2]);
            // pros::lcd::print(7 "Color: %f", optical.get_hue());

            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());

            pros::delay(50);
        }
    });

    // antiJamInit();
}

// Runs while the robot is disabled
void disabled() {}

// Runs after initialize if the robot is connected to field control
void competition_initialize() {}

ASSET(autonomous_txt); // TODO: add std functionality
ASSET(extra_txt);

void autonomous() {
    // chassis.calibrate(); //TODO: COMMENTED POUT BC TESTING IN INITIALIZE
    // chassis.setPose(0, 0, 0);
    // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    // TODO: COMMENTED POUT BC TESTING IN INITIALIZE
    // TODO: COMMENTED POUT BC TESTING IN INITIALIZE
    // TODO: COMMENTED POUT BC TESTING IN INITIALIZE
    // TODO: COMMENTED POUT BC TESTING IN INITIALIZE
    // TODO: COMMENTED POUT BC TESTING IN INITIALIZE
    initDebug();

    chassis.follow(autonomous_txt, extra_txt, 10, 1000000, true, false);

    // chassis.turnToHeading(90, 100000);
    // chassis.turnToHeading(180, 5000);
}

void opcontrol() {
    // chassis.calibrate(); // *: NEVER COMMENT OUT CALIBRATE OR SETPOSE OR ELSE IT WILL BREAK!!!!!!!!
    // chassis.setPose(0, 0, 0);

    // initDebug();

    // chassis.follow(interruptAutonomous_txt, interruptExtra_txt, 10, 1000000, true, false);

    // chassis.calibrate();
    // chassis.setPose(0, 0, 0);
    // TODO: COMMENTED OUT BC IN INITIALIZE
    // TODO: COMMENTED OUT BC IN INITIALIZE
    // TODO: COMMENTED OUT BC IN INITIALIZE
    // TODO: COMMENTED OUT BC IN INITIALIZE

    initO();

    int count = 1;
    int segCount = 1;

    pros::Task pd_task1(lbTask);
    pros::Task pd_task2(colorSort);
    pros::Task pd_task3(antiJam);

    // lbTask();

    while (true) {
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        chassis.arcade(leftY, rightX); // 0.9

        updateIntake();
        updateColorSort();
        updateClamp();
        updateDoink();
        updateLB();
        // lbTask();

        if (count == 5) { //*data written every 0.1 seconds
            writePose();
            writeAdditional();
            count = 1;
            fileO.flush();
            fileOTwo.flush();
        }

        // pros::lcd::print(4, std::to_string(round(leftMotors.get_voltage()) * 1000.0 / 1000.0).c_str(), 0);
        // pros::lcd::print(5, std::to_string(round(rightMotors.get_voltage()) * 1000.0 / 1000.0).c_str(), 0);
        // pros::lcd::print(6, std::to_string(round(leftMotors.get_voltage() + rightMotors.get_voltage()) * 1000.0 /
        // 1000.0 / 2).c_str(), 0);

        // if(segCount == 100) { //*new segment every give seconds
        //     section++;
        //     segCount = 1;
        // }

        count++;
        // segCount++;

        closeO();

        pros::delay(10);
    }
}