#include "main.h"
#include "globals.hpp"
#include "magic.hpp"
#include <iostream>
#include <fstream>
#include <string>

bool active = true;
// int count = 1;

void initO() {
    fileO.open("/usd/autonomous.txt");
    fileOTwo.open("/usd/extra.txt");
    if(!fileO && !fileOTwo) {
        controller.set_text(0, 0, "failed to open both");
        active = false;
    } else if (!fileO) {
        controller.set_text(0, 0, "pose failed to open");
        active = false;
    } else if (!fileOTwo) {
        controller.set_text(0, 0, "extra failed to open");
        active = false;
    }
    
    else {
        controller.set_text(0, 0, "opened");
        active = true;
    }
}

void initI() {
    fileI.open("/usd/autonomous.txt");
    fileITwo.open("/usd/extra.txt");
    if(!fileI && !fileITwo) {
        controller.set_text(0, 0, "failed to open both");
        active = false;
    } else if (!fileI) {
        controller.set_text(0, 0, "pose failed to open");
        active = false;
    } else if (!fileITwo) {
        controller.set_text(0, 0, "extra failed to open");
        active = false;
    }
    
    else {
        controller.set_text(0, 0, "opened");
        active = true;
    }
}


void closeO() {
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A) && active) {
        std::string dataLine = "\nendData";

        fileO << dataLine;
        fileOTwo << dataLine;

        fileO.flush();
        fileOTwo.flush();

        controller.set_text(0, 0, "wait");
        pros::delay(2500);

        fileO.close();
        fileOTwo.close();

        active = false;

        controller.set_text(0, 0, "file closed");
    }
}

void closeI() {
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A) && active) {
        fileI.close();
        fileITwo.close();

        active = false;

        controller.set_text(0, 0, "file closed");
    }
}

void write() {
    if(active) {
        std::string dataLine = "";
        dataLine.append(std::to_string(leftMotors.get_voltage()) + " ");
        dataLine.append(std::to_string(rightMotors.get_voltage()) + "\n");
        
        fileO << dataLine; //<< std::endl;
        
        fileO.flush();
    }
}

void writeControllerData() {
    if(active) {
        std::string dataLine = "";
        dataLine.append(std::to_string(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)) + " ");
        dataLine.append(std::to_string(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)) + "\n");

        fileO << dataLine;
        
        fileO.flush();
    }
}

void read() {
    if(active) {
        int voltages[2];
        
        for (int i=0; i<2; i++) {
            fileI >> voltages[i];
        }

        leftMotors.move_voltage(voltages[0]);
        rightMotors.move_voltage(voltages[1]);

        // driveTwo.move_voltage(voltages[1]);
    }
    
    /* if(active) {
        int voltage;
        fileI >> voltage;
        
        // double voltages[2];
        // for (int i=0; i<2; i++) {
        //     file >> voltages[i];
        // }

        driveOne.move_voltage(voltage);

        // driveTwo.move_voltage(voltages[1]);
    } */
}

void readControllerData() {
    if(active) {
        double leftInput;
        double rightInput;

        fileI >> leftInput;
        fileI >> rightInput;

        leftMotors.move_velocity((leftInput));
        rightMotors.move_velocity((leftInput));
    }
}

void runMotors() {
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
		driveOne.move_voltage(12000);
    } else {
		driveOne.move_voltage(0);
    }

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
        driveTwo.move_voltage(12000);
    } else {
        driveTwo.move_voltage(0);
    }
}

void driveBasic() {
    leftMotors.move_velocity(
        (controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)));
    rightMotors.move_velocity(
        (controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)));
}


void writePose() {
   std::string dataLine = "";
   lemlib::Pose pose = chassis.getPose();

   dataLine.append(std::to_string(pose.x) + ", ");
   dataLine.append(std::to_string(pose.y) + ", ");
   dataLine.append(std::to_string(pose.theta) + "\n");


   fileO << dataLine;
        
   fileO.flush();
}

void writeAdditional() {
    std::string dataLine = "";

    if(leftMotors.get_voltage() < 0 && rightMotors.get_voltage()<0)
        dataLine.append(1 + ", ");
    else
        dataLine.append(0 + ", ");

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) { //non toggle subsystem
        dataLine.append(1 + "\n");
    }
    else
        dataLine.append(0 + "\n");

    fileO << dataLine;
    
    fileOTwo.flush();
}