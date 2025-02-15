#include "globals.hpp"
#include "magic.hpp"
#include "intake.hpp"
#include "piston.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "ladybrown.hpp"

int section = 0;
bool buttonPressed = false;
float prevError;

bool active = false;

// std::vector<Waypoint> route;
// int count = 1;

void initO() {
    fileO.open("/usd/autonomous.txt", std::ios::out | std::ios::trunc);
    fileOTwo.open("/usd/extra.txt", std::ios::out | std::ios::trunc);
    if(!fileO || !fileOTwo) {
        controller.set_text(0, 0, "failed to open both");
    }
    
    else {
        controller.set_text(0, 0, "goated");
        active = true;
    }
}

void initInterrupt(int lastSection, int stopIndex) {
    fileInterrupt.open("/usd/interruptAutonomous.txt", std::ios::out | std::ios::trunc);
    fileInterruptTwo.open("/usd/interruptExtra.txt", std::ios::out | std::ios::trunc);
    //maybe move out of function

    fileI.open("/usd/autonomous.txt");
    fileITwo.open("/usd/extra.txt");

    if(!fileInterrupt || !fileInterruptTwo || !fileI || !fileITwo) {
        controller.set_text(0, 0, "failed to open");
        active = false;
    
    } else {
        controller.set_text(0, 0, "open successful");
        active = true;
    }

    std::string dataLine;

    for (int i = 0; i<stopIndex; i++) { //TODO: add data checks (like if it isn't getting data properly)
        std::getline(fileI, dataLine);
        fileInterrupt<<dataLine<<"\n";
        controller.set_text(0, 0, "copied fileI <3");


    }

    for (int j = 0; j<stopIndex; j++) {
        std::getline(fileITwo, dataLine);
        fileInterruptTwo<<dataLine<<"\n";
        controller.set_text(0, 0, "copied fileITwo <3");

    }

    section = lastSection + 1;

    fileInterrupt.flush();
    fileInterruptTwo.flush();

    fileI.close();
    fileITwo.close();

    controller.set_text(0, 0, "copied");

    pros::delay(3000);

    opcontrol();
}

void initDebug() {

    fileOThree.open("/usd/debug.txt");
    if(!fileOThree) {
        controller.set_text(0, 0, "failed to open");
    } else {
        controller.set_text(0, 0, "opened");
        active = true;
    }
}

void closeO() {
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
        std::string dataLine = "0, 0, 0.000000, 0, STOPPED, -1\nendData";
        fileOTwo << dataLine;
        
        dataLine.clear();

        dataLine.append(std::to_string((round(chassis.getPose().x*1000))/1000) + ", "); //*all rounded to 3 decimal places
        dataLine.append(std::to_string((round(chassis.getPose().y*1000))/1000) + ", ");
        dataLine.append(std::to_string((round(chassis.getPose().theta*1000))/1000) + ", ");
        dataLine.append("0\nendData");
        fileO << dataLine;

        fileO.flush();
        fileOTwo.flush();

        controller.set_text(0, 0, "wait                          ");
        pros::delay(2500);

        if (fileO.is_open() && fileOTwo.is_open()) {
            fileO << dataLine;
            fileOTwo << dataLine;
        }
        
        fileO.close();
        fileOTwo.close();
        pros::delay(100);

        active = false;

        controller.set_text(0, 0, "file closed");
    }
}

void closeOInterrupt() {
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
        std::string dataLine = "0, 0, 0.000000, 0, STOPPED, -1\nendData";
        fileInterruptTwo << dataLine;
        
        dataLine.clear();

        dataLine.append(std::to_string((round(chassis.getPose().x*1000))/1000) + ", "); //*all rounded to 3 decimal places
        dataLine.append(std::to_string((round(chassis.getPose().y*1000))/1000) + ", ");
        dataLine.append(std::to_string((round(chassis.getPose().theta*1000))/1000) + ", ");
        dataLine.append("0\nendData");
        fileInterrupt << dataLine;

        fileInterrupt.flush();
        fileInterruptTwo.flush();

        controller.set_text(0, 0, "wait                          ");
        pros::delay(2500);

        fileInterrupt.close();
        fileInterruptTwo.close();

        active = false;

        controller.set_text(0, 0, "file closed");
    }
}

void writePose() {
    std::string dataLine = "";
    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage(); 
    float adjusted = round((right+left) * 1000.0) / 1000.0 / 2;

    dataLine.append(std::to_string((round(chassis.getPose().x*1000))/1000) + ", "); //*all rounded to 3 decimal places
    dataLine.append(std::to_string((round(chassis.getPose().y*1000))/1000) + ", ");
    dataLine.append(std::to_string((round(chassis.getPose().theta*1000))/1000) + ", "); //*changed from speed to angle
    dataLine.append(std::to_string(adjusted) + "\n");

    fileO << dataLine;
    
    if(!fileO && active) {
        controller.set_text(0, 0, "write error");
    }
}

void writeAdditional() {

    //*0 is intake, 1 is mogo, 2 is lb, 3 is doinker, 4 is state, 5 is segment

    std::string dataLine = "";

    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage(); 
    float total = left + right;

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)) { //TODO: change button back to x
        if (!buttonPressed) {
            buttonPressed = true;
            section++;
        }
    } else {
        buttonPressed = false;
    }

    dataLine.append(std::to_string(intakeState) + ", ");
    dataLine.append(std::to_string(clampState) + ", ");
    dataLine.append(std::to_string(globalTarget) + ", ");
    dataLine.append(std::to_string(doinkState) + ", ");

    if(std::abs(total) < 600) { //TODO: tune stop bound

        if(std::abs(right) > 800 && std::abs(left) > 800) {
            if(right < 0)  {
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
    
    if(!fileInterrupt && active) {
        controller.set_text(0, 0, "write error");
    }
}

void writeInterruptAdditional() {
    
    //*0 is intake, 1 is mogo, 2 is lb, 3 is doinker, 4 is state, 5 is segment

    std::string dataLine = "";

    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage(); 
    float total = left + right;

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)) { //TODO: change button back
        if (!buttonPressed) {
            buttonPressed = true;
            section++;
        }
    } else {
        buttonPressed = false;
    }

    dataLine.append(std::to_string(intakeState) + ", ");
    dataLine.append(std::to_string(clampState) + ", ");
    dataLine.append(std::to_string(globalTarget) + ", ");
    dataLine.append(std::to_string(doinkState) + ", ");

    if(std::abs(total) < 600) { //TODO: tune stop bound

        if(std::abs(right) > 800 && std::abs(left) > 800) {
            if(right < 0)  {
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

    dataLine.append(std::to_string(section) + "\n");

    fileInterruptTwo << dataLine;
}

void reflect(bool x, bool y) {

    reflector.open("/usd/reflectAutonomous.txt", std::ios::out | std::ios::trunc);
    reflectorTwo.open("/usd/reflectExtra.txt", std::ios::out | std::ios::trunc);

    fileI.open("/usd/autonomous.txt");
    fileITwo.open("/usd/extra.txt");

    if(!reflector || !reflectorTwo || !fileI || !fileITwo) {
        controller.set_text(0, 0, "failed to open");
        return;
    } else {
        controller.set_text(0, 0, "open successful");\
    }

    std::string tempData;

    std::cout<<"light\n";

    pros::delay(2000);

    while (std::getline(fileI, tempData)) {
        std::string dataLine;
        std::vector<std::string> pointData = readElementMagic(tempData, ", ");

        for(int i = 0; i<4; i++) {
        
            if(i == 0) { //x coord
                if (y) {
                    dataLine.append(std::to_string(std::stof(pointData.at(i)) * -1) + ", ");
                } else {
                    dataLine.append(pointData.at(i) + ", ");
                }
            }

            else if(i == 1) { //y coord
                if (x) {
                    dataLine.append(std::to_string(std::stof(pointData.at(i)) * -1) + ", ");
                } else {
                    dataLine.append(pointData.at(i) + ", ");
                }
            }

            else if(i == 2) { //heading
                if (x) {
                    dataLine.append(std::to_string(std::stof(pointData.at(i)) * -1) + ", ");
                } else if (y) {
                    dataLine.append(std::to_string(std::stof(pointData.at(i)) * -1) + ", ");
                } else { //for either double reflection or no reflection
                    dataLine.append(std::to_string(std::stof(pointData.at(i))) + ", ");
                }
            }

            else if(i == 3) { //vel
                dataLine.append(pointData.at(i) + "\n");
            }
        }
        reflector<<dataLine;
        reflector.flush();

        if(!reflector) {
            std::cout<<"reflector fail\n";
            return;
        }

        if(!fileI) {
            std::cout<<"input fail \n";
            return;
        }
    }

    std::cout<<"light\n";

    reflector<<"endData";
    reflector.flush();

    while (std::getline(fileITwo, tempData)) {

        std::string dataLine;

        std::vector<std::string> pointData = readElementMagic(tempData, ", ");
        for(int i = 0; i<pointData.size(); i++) {
            if(i == pointData.size() - 1) {
                dataLine.append(pointData.at(i) + "\n");
            } else {
                dataLine.append(pointData.at(i) + ", ");
            }
        }
        
        reflectorTwo<<dataLine;
        reflectorTwo.flush();

        if(!reflectorTwo) {
            std::cout<<"reflector fail 2\n";
            return;
        }

        if(!fileITwo) {
            std::cout<<"input fail 2\n";
            return;
        }
    }

    std::cout<<"light\n";

    reflectorTwo<<"endData";

    reflector.flush();
    reflectorTwo.flush();

    reflectorTwo.close();
    reflector.close();
    fileI.close();
    fileITwo.close();

    std::cout<<"light\n";

    controller.clear_line(0);
    controller.set_text(0, 0, "reflected"); 

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