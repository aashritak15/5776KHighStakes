#include "main.h"
#include "globals.hpp"
#include "magic.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

bool active = true;
// std::vector<Waypoint> route;
// int count = 1;

void initO() {
    pros::lcd::print(7, "?????");
    fileO.open("/usd/autonomous.txt");
    fileOTwo.open("/usd/extra.txt");
    if(!fileO && !fileOTwo) {
        pros::lcd::print(7, "both are cooked");
        controller.set_text(0, 0, "failed to open both");
        active = false;
    } else if (!fileO) {
        pros::lcd::print(7, "pose is cooked");
        controller.set_text(0, 0, "pose failed to open");
        active = false;
    } else if (!fileOTwo) {
        pros::lcd::print(7, "extra is cooked");
        controller.set_text(0, 0, "extra failed to open");
        active = false;
    }
    
    else {
        pros::lcd::print(7, "goated");
        controller.set_text(0, 0, "goated");
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

    // readRouteFile(); 
}


void closeO() {
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_B) && active) {
        std::string dataLine = "endData";

        fileO << dataLine;
        fileOTwo << dataLine;

        fileO.flush();
        fileOTwo.flush();

        controller.set_text(0, 0, "wait                          ");
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

void writePose() {
    std::string dataLine = ""; //TODO: CHANGE GETPOSE BACK
    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage();   

    //rounds to 3 decimal places (idk if that helps)
    dataLine.append(std::to_string((round(chassis.getPose().x*1000))/1000) + ", ");
    dataLine.append(std::to_string((round(chassis.getPose().y*1000))/1000) + ", ");
    dataLine.append(std::to_string((right+left)/2.0*127.0/12000.0) + "\n");

    //dataLine.append(std::to_string(pose.x) + ", ");
    //dataLine.append(std::to_string(pose.y) + ", ");
    //dataLine.append(std::to_string(pose.theta) + "\n");


    fileO << dataLine;
        
    //fileO.flush();
}

void writeAdditional() {
    std::string dataLine = "";

    if(leftMotors.get_voltage() < 0 && rightMotors.get_voltage() < 0)
        dataLine.append("1, ");
    else
        dataLine.append("0, ");

    // if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) { //non toggle subsystem
    //     dataLine.append("1, ");
    // }
    // else {
    //     dataLine.append("0, ");
    // }

    dataLine.append(std::to_string(intakeState) + "\n");

    fileOTwo << dataLine;

    // fileOTwo.flush();
}

//jerry stuff 

// void readRouteFile() {
//     std::ifstream pathFile("/usd/path.txt");
//     if (!pathFile.is_open()) {
//         std::cerr << "Failed to open path file" << std::endl;
//         return;
//     }

//     Waypoint waypoint;
//     while (pathFile >> waypoint.x >> waypoint.y >> waypoint.theta) {
//         path.push_back(waypoint);
//     }
//     pathFile.close();
// }

// void followRoute(const std::vector<Waypoint>& path) {
//     for (const auto& waypoint : path) {
//         moveToWaypoint(waypoint);
//     }
// }

// //not right i need to fix 
// void moveToWaypoint(const Waypoint& waypoint) {
//     leftMotors.move_velocity(waypoint.x);  
//     rightMotors.move_velocity(waypoint.y);  
// }