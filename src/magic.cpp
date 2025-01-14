#include "main.h"
#include "globals.hpp"
#include "magic.hpp"
#include "intake.hpp"
#include "piston.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

bool active = true;
int prevIntakeState = 0;
int prevClampState = 0;

// std::vector<Waypoint> route;
// int count = 1;

void initO() {
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

void initDebug() {
    fileOThree.open("/usd/debug.txt");
    if(!fileOThree) {
        controller.set_text(0, 0, "failed to open");
    } else {
        controller.set_text(0, 0, "opened");
    }
}

// void initReplace() {
//     newPoseFile.open("/usd/newAutonomous.txt");
//     newExtraFile.open("/usd/newExtra.txt");
//     if(!newPoseFile && !newExtraFile) {
//         pros::lcd::print(7, "both are cooked");
//         controller.set_text(0, 0, "failed to open both");
//         active = false;
//     } else if (!newPoseFile) {
//         pros::lcd::print(7, "pose is cooked");
//         controller.set_text(0, 0, "pose failed to open");
//         active = false;
//     } else if (!newExtraFile) {
//         pros::lcd::print(7, "extra is cooked");
//         controller.set_text(0, 0, "extra failed to open");
//         active = false;
//     }
    
//     else {
//         pros::lcd::print(7, "goated");
//         controller.set_text(0, 0, "goated");
//         active = true;
//     }
// }

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

void writePose() {
    std::string dataLine = ""; //TODO: CHANGE GETPOSE BACK
    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage(); 
    float adjusted = round((right+left) / 2.0 * 127.0 / 12000.0 * 1000) / 1000;

    dataLine.append(std::to_string((round(chassis.getPose().x*1000))/1000) + ", "); //*all rounded to 3 decimal places
    dataLine.append(std::to_string((round(chassis.getPose().y*1000))/1000) + ", ");
    dataLine.append(std::to_string((round(chassis.getPose().theta*1000))/1000) + ", "); //*changed from speed to angle
    dataLine.append(std::to_string(adjusted) + "\n");

    fileO << dataLine;
}

void writeAdditional() { //TODO: OPTIMIZE
    std::string dataLine = "";

    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage(); 
    float adjLeft = right/2.0*127.0/12000.0;
    float adjRight = left/2.0*127.0/12000.0;
    float adjTotal = adjLeft + adjRight;

    dataLine.append(std::to_string(intakeState) + ", ");

    dataLine.append(std::to_string(clampState) + ", ");

    if(std::abs(adjTotal) < 5) {

        if(std::abs(adjRight) > 5 && std::abs(adjLeft) > 5) {
            if(adjRight > 0)  {//TODO: CHECK {
                dataLine.append("TURNING CW\n");
            } else if (adjRight < 0) {
                dataLine.append("TURNING CCW\n");
            }
        } else if(!(prevIntakeState == intakeState) || !(prevClampState == clampState)) {
            dataLine.append("SUBSYS\n");
        } 
        else {dataLine.append("STOPPED\n");}

    } else {
        dataLine.append("GOING\n");
    }

    prevIntakeState = intakeState;
    prevClampState = clampState;

    fileOTwo << dataLine;
}


void addSegment() {
    std::ifstream file0("static/newAuton.txt");
    std::ifstream file0Two("static/newExtra.txt");
    std::ifstream coords("static/coords");
    float[3] pose;
    string temp="";
    for(int i=0;i<3;i++) {
        coords>>temp;
        pose[i] = std::stof(temp);
        temp="";
    }
    chassis.setPose(pose[0], pose[1], pose[2]);
}

void mergeFiles(std::ifstream& file1, std::ifstream& file2) {
    std::ofstream mergedFile("static/mergedFile.txt");

    if (!file1.is_open() || !file2.is_open() || !mergedFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return 1;
    }

    std::string line;

    // Copy contents of file1 to mergedFile
    while (std::getline(file1, line)) {
        mergedFile << line << std::endl;
    }

    // Copy contents of file2 to mergedFile
    while (std::getline(file2, line)) {
        mergedFile << line << std::endl;
    }

    file1.close();
    file2.close();
    mergedFile.close();

    std::cout << "Files merged successfully!" << std::endl;
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