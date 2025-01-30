#include "globals.hpp"
#include "magic.hpp"
#include "intake.hpp"
#include "piston.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

int section = 0;
bool buttonPressed = false;
float prevError;

// std::vector<Waypoint> route;
// int count = 1;

void initO() {
    fileO.open("/usd/autonomous.txt", std::ios::out | std::ios::trunc);
    fileOTwo.open("/usd/extra.txt", std::ios::out | std::ios::trunc);
    if(!fileO || !fileOTwo) {
        pros::lcd::print(7, "failed to open both");
        controller.set_text(0, 0, "failed to open both");
    }
    
    else {
        pros::lcd::print(7, "goated");
        controller.set_text(0, 0, "goated");
    }
}

// void initInterrupt(int lastSection, int stopIndex) {
//     fileInterrupt.open("/usd/revAutonomous", std::ios::out | std::ios::trunc);
//     fileInterruptTwo.open("/usd/revExtra", std::ios::out | std::ios::trunc);

//     fileI.open("/usd/autonomous.txt");
//     fileITwo.open("/usd/extra.txt");

//     if(!fileInterrupt || !fileInterruptTwo || fileI || fileITwo) {
//         pros::lcd::print(7, "failed to open");
//         controller.set_text(0, 0, "failed to open");
//     } else {
//         controller.set_text(0, 0, "open successful");
//     }

//     std::string dataLine;

//     for (int i = 0; i<stopIndex; i++) { //TODO: add data checks (like if it isn't getting data properly)
//         std::getline(fileI, dataLine);
//         fileInterrupt<<dataLine;
//     }

//     for (int j = 0; j<stopIndex; j++) {
//         std::getline(fileITwo, dataLine);
//         fileInterruptTwo<<dataLine;
//     }

//     section = lastSection + 1; //TODO: check logic

//     controller.set_text(0, 0, "copied");
// }

void initButtonInterrupt(int stopIndex) {
    fileInterrupt.open("/usd/revAutonomous", std::ios::out | std::ios::trunc);
    fileInterruptTwo.open("/usd/revExtra", std::ios::out | std::ios::trunc);
    //maybe move out of function

    fileI.open("/usd/autonomous.txt");
    fileITwo.open("/usd/extra.txt");

    if(!fileInterrupt || !fileInterruptTwo || fileI || fileITwo) {
        pros::lcd::print(7, "failed to open");
        controller.set_text(0, 0, "failed to open");
    } else {
        controller.set_text(0, 0, "open successful");
    }

    std::string dataLine;

    for (int i = 0; i<stopIndex; i++) { //TODO: add data checks (like if it isn't getting data properly)
        std::getline(fileI, dataLine);
        fileInterrupt<<dataLine;
    }

    for (int j = 0; j<stopIndex; j++) {
        std::getline(fileITwo, dataLine);
        fileInterruptTwo<<dataLine;
    }

    section++;

    controller.set_text(0, 0, "copied");
}

void initDebug() {
    fileOThree.open("/usd/debug.txt");
    if(!fileOThree) {
        controller.set_text(0, 0, "failed to open");
    } else {
        controller.set_text(0, 0, "opened");
    }
}

void closeO() {
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
        std::string dataLine = "endData";

        fileO << dataLine;
        fileOTwo << dataLine;

        fileO.flush();
        fileOTwo.flush();

        controller.set_text(0, 0, "wait                          ");
        pros::delay(2500);

        fileO.close();
        fileOTwo.close();

        controller.set_text(0, 0, "file closed");
    }
}

void closeOInterrupt() {
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
        std::string dataLine = "endData";

        fileInterrupt << dataLine;
        fileInterruptTwo << dataLine;

        fileInterrupt.flush();
        fileInterruptTwo.flush();

        controller.set_text(0, 0, "wait                          ");
        pros::delay(2500);

        fileInterrupt.close();
        fileInterrupt.close();
        fileI.close();
        fileITwo.close();

        controller.set_text(0, 0, "file closed");
    }
}

void writePose() {
    std::string dataLine = ""; //TODO: CHANGE GETPOSE BACK
    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage(); 
    float adjusted = round((right+left) * 1000.0) / 1000.0;

    dataLine.append(std::to_string((round(chassis.getPose().x*1000))/1000) + ", "); //*all rounded to 3 decimal places
    dataLine.append(std::to_string((round(chassis.getPose().y*1000))/1000) + ", ");
    dataLine.append(std::to_string((round(chassis.getPose().theta*1000))/1000) + ", "); //*changed from speed to angle
    dataLine.append(std::to_string(adjusted) + "\n");

    fileO << dataLine;
    
    if(!fileO) {
        controller.set_text(0, 0, "write error");
    }
}

void writeAdditional() {
    std::string dataLine = "";

    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage(); 
    float total = left + right;

    dataLine.append(std::to_string(intakeState) + ", ");
    dataLine.append(std::to_string(clampState) + ", ");

    if(std::abs(total) < 600) { //TODO: TUNE THIS VALUE

        if(std::abs(right) > 600 && std::abs(left) > 600) {
            if(right < 0)  { //TODO: CHECK 
                dataLine.append("TURNING CW, ");
            } else if (right > 0) {
                dataLine.append("TURNING CCW, ");
            }
        } //else if(!(prevIntakeState == intakeState) || !(prevClampState == clampState)) {
        //     dataLine.append("SUBSYS, ");
        // } 

        else {dataLine.append("STOPPED, ");}

    } else {
        dataLine.append("GOING, ");
    }

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) { //TODO: CHANGE BUTTON
        if (!buttonPressed) {
            buttonPressed = true;
            section++;
        }
    } else {
        buttonPressed = false;
    }

    dataLine.append(std::to_string(section) + "\n");

    fileOTwo << dataLine;
}

void writeInterruptPose() {
    std::string dataLine = "";
    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage(); 
    float adjusted = round((right+left) * 1000.0) / 1000.0;

    dataLine.append(std::to_string((round(chassis.getPose().x*1000))/1000) + ", "); //*all rounded to 3 decimal places
    dataLine.append(std::to_string((round(chassis.getPose().y*1000))/1000) + ", ");
    dataLine.append(std::to_string((round(chassis.getPose().theta*1000))/1000) + ", "); //*changed from speed to angle
    dataLine.append(std::to_string(adjusted) + "\n");

    fileInterrupt << dataLine;
    
    if(!fileInterrupt) {
        controller.set_text(0, 0, "write error");
    }
}

void writeInterruptAdditional() {
    
    std::string dataLine = "";

    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage(); 
    float total = left + right;

    dataLine.append(std::to_string(intakeState) + ", ");
    dataLine.append(std::to_string(clampState) + ", ");

    if(std::abs(total) < 600) {

        if(std::abs(right) > 600 && std::abs(left) > 600) {
            if(right > 0)  {
                dataLine.append("TURNING CW, ");
            } else if (right < 0) {
                dataLine.append("TURNING CCW, ");
            }
        } //else if(!(prevIntakeState == intakeState) || !(prevClampState == clampState)) {
        //     dataLine.append("SUBSYS, ");
        // } 

        else {dataLine.append("STOPPED, ");}

    } else {
        dataLine.append("GOING, ");
    }

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) { //TODO: CHANGE BUTTON
        if (!buttonPressed) {
            buttonPressed = true;
            section++;
        }
    } else {
        buttonPressed = false;
    }

    dataLine.append(std::to_string(section) + "\n");

    fileInterruptTwo << dataLine;
}

void reflect(bool x, bool y) {

    reflector.open("/usd/reflectAutonomous", std::ios::out | std::ios::trunc);
    reflector.open("/usd/reflectExtra", std::ios::out | std::ios::trunc);

    fileI.open("/usd/autonomous.txt");
    fileITwo.open("/usd/extra.txt");

    if(!fileInterrupt || !fileInterruptTwo || fileI || fileITwo) {
        pros::lcd::print(7, "failed to open");
        controller.set_text(0, 0, "failed to open");
    } else {
        controller.set_text(0, 0, "open successful");
    }

    std::string dataLine;
    std::string tempData;

    while (std::getline(fileI, tempData)) {
        std::vector<std::string> pointData = readElementMagic(tempData, ", ");
        for(int i = 0; i<pointData.size(); i++) {

            if(i == 1) {
                if(x) {
                    dataLine.append(std::to_string(stof(pointData.at(i)) * -1));
                    dataLine.append(std::to_string(stof(pointData.at(2)) * -1));
                }
            } else if(i == 0) {
                if(y) {
                    dataLine.append(std::to_string(stof(pointData.at(i)) * -1));
                    dataLine.append(std::to_string(stof(pointData.at(2)) * -1));
                }
            }

            dataLine.append(pointData.at(i));
        }
        
        reflector<<dataLine;
    }

    while (std::getline(fileITwo, tempData)) {
        std::vector<std::string> pointData = readElementMagic(tempData, ", ");
        for(int i = 0; i<pointData.size(); i++) {
            dataLine.append(pointData.at(i));
        }
        
        reflectorTwo<<dataLine;
    }

    controller.set_text(0, 0, "reflected"); 
}

void rerunPIDs() {
    float avgVel = ((leftMotors.get_voltage() + rightMotors.get_voltage()) / 2.0);
    float targetVel = 400.0;
    prevError = targetVel;

    while(true) {
        float Kd = 0;
        float Kp = 1;
        
        avgVel = ((leftMotors.get_voltage() + rightMotors.get_voltage()) / 2.0);
        
        float error = targetVel - avgVel;
        float errorChange = prevError - error;

        float proportional = error * Kp;
        float derivative = errorChange * Kd;

        targetVel = proportional + derivative;

        leftMotors.move_voltage(targetVel);
        rightMotors.move_voltage(targetVel);

        pros::delay(10);
    }
}

std::vector<std::string> readElementMagic(const std::string& input, const std::string& delimiter) {
    std::string token;
    std::string s = input;
    std::vector<std::string> output;
    size_t pos = 0;

    // main loop
    while ((pos = s.find(delimiter)) != std::string::npos) { // while there are still delimiters in the string
        token = s.substr(0, pos); // processed substring
        output.push_back(token);
        s.erase(0, pos + delimiter.length()); // remove the read substring
    }

    output.push_back(s); // add the last element to the returned string

    return output;
}