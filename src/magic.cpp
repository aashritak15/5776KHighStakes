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
    if (!fileO || !fileOTwo) {
        controller.set_text(0, 0, "failed to open both");
    }

    else {
        controller.set_text(0, 0, "goated");
        active = true;
    }
}

void initInterrupt(int lastSection, int stopIndex) {
    std::cout << "DEBUG: initInterrupt() started\n";

    fileInterrupt.open("/usd/interruptAutonomous.txt", std::ios::out | std::ios::trunc);
    fileInterruptTwo.open("/usd/interruptExtra.txt", std::ios::out | std::ios::trunc);
    // maybe move out of function

    fileI.open("/usd/autonomous.txt");
    fileITwo.open("/usd/extra.txt");

    if (!fileInterrupt || !fileInterruptTwo || !fileI || !fileITwo) {
        controller.set_text(0, 0, "failed to open");
        active = false;

    } else {
        controller.set_text(0, 0, "open successful");
        active = true;
    }

    std::string dataLine;

    for (int i = 0; i < stopIndex; i++) { // TODO: add data checks (like if it isn't getting data properly)
        std::getline(fileI, dataLine);
        fileInterrupt << dataLine << "\n";
        controller.set_text(0, 0, "copied fileI <3");
    }

    for (int j = 0; j < stopIndex; j++) {
        std::getline(fileITwo, dataLine);
        fileInterruptTwo << dataLine << "\n";
        controller.set_text(0, 0, "copied fileITwo <3");
    }

    section = lastSection + 1;

    fileInterrupt.flush();
    fileInterruptTwo.flush();

    fileI.close();

    if (fileITwo.is_open()) {
        fileITwo.close();
        pros::delay(50);
    }

    controller.set_text(0, 0, "copied");

    pros::delay(300);

    std::cout << "DEBUG: initInterrupt() finished, transitioning to opcontr\n";

    opcontrol();
}

void initDebug() {
    fileOThree.open("/usd/debug.txt");
    if (!fileOThree) {
        controller.set_text(0, 0, "failed to open");
    } else {
        controller.set_text(0, 0, "opened");
        active = true;
    }
}

void closeO() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
        std::string dataLine = "0, 0, 0.000000, 0, 0, 0, STOPPED, -1\nendData";
        fileOTwo << dataLine;

        std::string dataLine2;

        dataLine2.append(std::to_string((round(chassis.getPose().x * 1000)) / 1000) +
                         ", "); //*all rounded to 3 decimal places
        dataLine2.append(std::to_string((round(chassis.getPose().y * 1000)) / 1000) + ", ");
        dataLine2.append(std::to_string((round(chassis.getPose().theta * 1000)) / 1000) + ", ");
        dataLine2.append("0\nendData");
        fileO << dataLine2;

        fileO.flush();
        fileOTwo.flush();

        controller.set_text(0, 0, "wait                          ");
        pros::delay(2500);

        // if (fileO.is_open() && fileOTwo.is_open()) {
        //     fileO << dataLine;
        //     fileOTwo << dataLine;
        // }

        if (fileO.is_open()) fileO.close();
        if (fileOTwo.is_open()) fileOTwo.close();

        //TODO: all past here is filtering!

        pros::delay(100);

        active = false;

        controller.set_text(0, 0, "file closed");

        // pros::delay(1000);

        // controller.set_text(0, 0, "running filters");

        // pros::delay(1000);

        // filterAuton();

        // pros::delay(1000);

        // controller.set_text(0, 0, "filters cleaned");
    }
}

void closeOInterrupt() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
        std::string dataLine = "0, 0, 0.000000, 0, 0, 0, STOPPED, -1\nendData";
        fileInterruptTwo << dataLine;

        dataLine = "";
        dataLine.clear();
        pros::delay(200);
        // dataLine.clear();

        dataLine.append(std::to_string((round(chassis.getPose().x * 1000)) / 1000) +
                        ", "); //*all rounded to 3 decimal places
        dataLine.append(std::to_string((round(chassis.getPose().y * 1000)) / 1000) + ", ");
        dataLine.append(std::to_string((round(chassis.getPose().theta * 1000)) / 1000) + ", ");
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
    float adjusted = round((right + left) * 1000.0) / 1000.0 / 2;

    dataLine.append(std::to_string((round(chassis.getPose().x * 1000)) / 1000) +
                    ", "); //*all rounded to 3 decimal places
    dataLine.append(std::to_string((round(chassis.getPose().y * 1000)) / 1000) + ", ");
    dataLine.append(std::to_string((round(chassis.getPose().theta * 1000)) / 1000) +
                    ", "); //*changed from speed to angle
    dataLine.append(std::to_string(adjusted) + "\n");

    fileO << dataLine;

    if (!fileO && active) { controller.set_text(0, 0, "write error"); }
}

void writeAdditional() {
    //*0 is intake, 1 is mogo, 2 is lb, 3 is doinker right, 4 is state, 5 is segment, 6 is doinker left

    std::string dataLine = "";

    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage();
    float total = left + right;

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) { // TODO: change button back to x
        if (!buttonPressed) {
            buttonPressed = true;
            section++;
        }
    } else {
        buttonPressed = false;
    }

    dataLine.append(std::to_string(intakeState) + ", "); //0
    dataLine.append(std::to_string(clampState) + ", "); //1
    dataLine.append(std::to_string(globalTarget) + ", "); //2
    dataLine.append(std::to_string(doinkRightState) + ", "); //3
    dataLine.append(std::to_string(intakePistonState) + ", "); //4
    dataLine.append(std::to_string(doinkLeftState) + ", "); //5

    if (std::abs(total) < 600) { // TODO: tune stop bound

        //change this condition - doesn't always accurately mark turns (doesnt end turn when it should)
        if (std::abs(right) > 800 && std::abs(left) > 800) { 
            if (right < 0) {
                dataLine.append("TURNING CW, ");
            } else if (right > 0) {
                dataLine.append("TURNING CCW, ");
            }
        } // else if(!(prevIntakeState == intakeState) || !(prevClampState == clampState)) {
        //     dataLine.append("SUBSYS, ");
        // }

        else {
            dataLine.append("STOPPED, ");
        }

    } else {
        dataLine.append("GOING, ");
    }

    dataLine.append(std::to_string(section)+"\n");

    fileOTwo << dataLine;
}

void writeInterruptPose() {
    std::string dataLine = "";
    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage();
    float adjusted = round((right + left) * 1000.0) / 1000.0;

    dataLine.append(std::to_string((round(chassis.getPose().x * 1000)) / 1000) +
                    ", "); //*all rounded to 3 decimal places
    dataLine.append(std::to_string((round(chassis.getPose().y * 1000)) / 1000) + ", ");
    dataLine.append(std::to_string((round(chassis.getPose().theta * 1000)) / 1000) +
                    ", "); //*changed from speed to angle
    dataLine.append(std::to_string(adjusted) + "\n");

    fileInterrupt << dataLine;

    if (!fileInterrupt && active) { controller.set_text(0, 0, "write error"); }
}

void writeInterruptAdditional() {
    //*0 is intake, 1 is mogo, 2 is lb, 3 is doinker, 4 is state, 5 is segment

    std::string dataLine = "";

    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage();
    float total = left + right;

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) { // TODO: change button back
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
    dataLine.append(std::to_string(doinkRightState) + ", ");
    dataLine.append(std::to_string(doinkLeftState) + ", ");
    dataLine.append(std::to_string(intakePistonState)+"\n");    

    if (std::abs(total) < 600) { // TODO: tune stop bound

        if (std::abs(right) > 800 && std::abs(left) > 800) {
            if (right < 0) {
                dataLine.append("TURNING CW, ");
            } else if (right > 0) {
                dataLine.append("TURNING CCW, ");
            }
        } // else if(!(prevIntakeState == intakeState) || !(prevClampState == clampState)) {
        //     dataLine.append("SUBSYS, ");
        // }

        else {
            dataLine.append("STOPPED, ");
        }

    } else {
        dataLine.append("GOING, ");
    }

    dataLine.append(std::to_string(section) + "\n");

    fileInterruptTwo << dataLine;
}

void reflect(bool x, bool y) { // TODO: remove extra reflection
    reflector.open("/usd/reflectAutonomous.txt", std::ios::out | std::ios::trunc);

    fileI.open("/usd/autonomous.txt");

    if (!reflector || !reflectorTwo || !fileI || !fileITwo) {
        controller.set_text(0, 0, "failed to open");
        std::cout<<"failed to open";
        return;
    } else {
        controller.set_text(0, 0, "open successful");
        std::cout<<"failed to open";
    }

    std::string tempData;

    std::cout << "opened all\n";

    pros::delay(1000);

    while (std::getline(fileI, tempData)) {
        std::string dataLine;
        std::vector<std::string> pointData = readElementMagic(tempData, ", ");

        for (int i = 0; i < 4; i++) {
            if (i == 0) { // x coord
                if (y) {
                    dataLine.append(std::to_string(std::stof(pointData.at(i)) * -1) + ", ");
                } else {
                    dataLine.append(pointData.at(i) + ", ");
                }
            }

            else if (i == 1) { // y coord
                if (x) {
                    dataLine.append(std::to_string(std::stof(pointData.at(i)) * -1) + ", ");
                } else {
                    dataLine.append(pointData.at(i) + ", ");
                }
            }

            else if (i == 2) { // heading
                if (x) {
                    dataLine.append(std::to_string(std::stof(pointData.at(i)) * -1) + ", ");
                } else if (y) {
                    dataLine.append(std::to_string(std::stof(pointData.at(i)) * -1) + ", ");
                } else { // for either double reflection or no reflection
                    dataLine.append(std::to_string(std::stof(pointData.at(i))) + ", ");
                }
            }

            else if (i == 3) { // vel
                dataLine.append(pointData.at(i) + "\n");
            }

            std::cout << pointData.at(i) << "\n";
        }

        reflector << dataLine;

        std::cout << dataLine << "\n";

        if (!reflector) {
            std::cout << "reflector fail\n";
            return;
        }

        if (!fileI) {
            std::cout << "input fail\n";
            return;
        }

        tempData.clear();
    }

    std::cout << "reflection finished\n";

    reflector << "endData";

    reflector.flush();
    reflector.close();
    fileI.close();

    pros::delay(1000);

    std::cout << "closed\n";

    controller.set_text(0, 0, "reflected      ");
}

// parse a string using a delimiter

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

// general read/write fns

std::vector<std::string> readAutonFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) { lines.push_back(line); }
    file.close();
    return lines;
}

std::vector<std::vector<std::string>> readExtraFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::vector<std::string>> lines;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> pointInput = readElementMagic(line, ", ");
        lines.push_back(pointInput);
    }
    file.close();
    return lines;
}

// write the vector of strings into a file
void writeFileExtra(const std::string& filename, const std::vector<std::vector<std::string>>& data) {
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    for (const auto& row : data) {
        std::string line = "";
        for (size_t i = 0; i < row.size(); i++) {
            line += row[i];
            if (i < row.size() - 1) line += ", ";
        }
        file << line << "\n";
    }

    file.close();
}

void writeFileAuton(const std::string& filename, const std::vector<std::string>& data) {
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    for (const auto& row : data) {
        std::string line = "";
        for (size_t i = 0; i < row.size(); i++) {
            line += row[i];
            if (i < row.size() - 1) line += ", ";
        }
        file << line << "\n";
    }

    file.close();
}

// filters stuff

void removeIsolatedTurns(std::vector<std::vector<std::string>>& extra, std::vector<std::string>& autonomous) {
    controller.set_text(0,0,"removing isolated turns");
    std::vector<std::vector<std::string>> cleaned_extra;
    std::vector<std::string> cleaned_autonomous;
    for (size_t i = 0; i < extra.size(); i++) {
        if ((extra[i][6] == "TURNING CW," || extra[i][6] == "TURNING CCW,") &&
            ((i == 0 || (extra[i - 1][6] != "TURNING CW," && extra[i - 1][6] != "TURNING CCW,")) &&
             (i == extra.size() - 1 || (extra[i + 1][6] != "TURNING CW," && extra[i + 1][6] != "TURNING CCW,")))) {
            continue;
        }
        cleaned_extra.push_back(extra[i]);
        cleaned_autonomous.push_back(autonomous[i]);
    }
    extra = cleaned_extra;
    autonomous = cleaned_autonomous;
    controller.set_text(0,0,"done removing isolated turns");
}

void stoppedSequences(std::vector<std::vector<std::string>>& extra, std::vector<std::string>& autonomous) {
    controller.set_text(0,0,"removing stopped sequences");
    std::vector<std::vector<std::string>> cleaned_extra;
    std::vector<std::string> cleaned_autonomous;
    int stop_count = 0;
    for (size_t i = 0; i < extra.size(); i++) {
        if (extra[i][6].find("STOPPED") != std::string::npos) {
            stop_count++;
            if (stop_count > 20) continue; // maximum of 20
        } else {
            stop_count = 0;
        }
        cleaned_extra.push_back(extra[i]);
        cleaned_autonomous.push_back(autonomous[i]);
    }
    extra = cleaned_extra;
    autonomous = cleaned_autonomous;
    controller.set_text(0,0,"done removing stopped sequences");
}

// remove isolated stops

void removeIsolatedStopped(std::vector<std::vector<std::string>>& extra, std::vector<std::string>& autonomous) {
    controller.set_text(0,0,"removing isolated stops");
    std::vector<std::vector<std::string>> cleaned_extra;
    std::vector<std::string> cleaned_autonomous;
    for (size_t i = 0; i < extra.size(); i++) {
        if (extra[i][6] == "STOPPED," &&
            ((i == 0 || extra[i - 1][4] != "STOPPED,") && (i == extra.size() - 1 || extra[i + 1][4] != "STOPPED,"))) {
            continue;
        }
        cleaned_extra.push_back(extra[i]);
        cleaned_autonomous.push_back(autonomous[i]);
    }
    extra = cleaned_extra;
    autonomous = cleaned_autonomous;
    controller.set_text(0,0,"done removing isolated stops");
}

void optimizeTurns(std::vector<std::vector<std::string>>& extra, std::vector<std::string>& autonomous) {
    controller.set_text(0,0,"optimizing turns");
    std::vector<std::vector<std::string>> cleaned_extra;
    std::vector<std::string> cleaned_autonomous;
    size_t i = 0;
    while (i < extra.size()) {
        if (extra[i][6] == "TURNING CW," || extra[i][6] == "TURNING CCW,") {
            size_t start = i;
            while (i < extra.size() && (extra[i][6] == "TURNING CW," || extra[i][6] == "TURNING CCW,")) { i++; }
            size_t end = i - 1;
            cleaned_extra.push_back(extra[start]); // Keep first turn
            cleaned_extra.push_back(extra[end]); // Keep last turn
            cleaned_autonomous.push_back(autonomous[start]);
            cleaned_autonomous.push_back(autonomous[end]);
        } else {
            cleaned_extra.push_back(extra[i]);
            cleaned_autonomous.push_back(autonomous[i]);
        }
        i++;
    }
    extra = cleaned_extra;
    autonomous = cleaned_autonomous;
    controller.set_text(0,0,"done optimizing turns");
}


std::vector<lemlib::Pose> getData(const asset& path) {
    std::vector<lemlib::Pose> robotPath;
    // format data from the asset
    const std::string data(reinterpret_cast<char*>(path.buf), path.size);
    const std::vector<std::string> dataLines = readElementMagic(data, "\n");

    // read the points until 'endData' is read
    for (std::string line : dataLines) {
        if (line == "endData" || line == "endData\r") break;

        const std::vector<std::string> pointInput = readElementMagic(line, ", "); // parse line

        lemlib::Pose pathPoint(0, 0);
        pathPoint.x = std::stof(pointInput.at(0)); // x position
        pathPoint.y = std::stof(pointInput.at(1)); // y position
        pathPoint.theta = std::stof(pointInput.at(2)); // heading
        robotPath.push_back(pathPoint); // save data
    }

    return robotPath;
}

std::vector<std::vector<std::string>> getSubData(const asset& sub) {
    // format data from the asset
    const std::string data(reinterpret_cast<char*>(sub.buf), sub.size);
    const std::vector<std::string> dataLines = readElementMagic(data, "\n");
    std::vector<std::vector<std::string>> pointInput;

    // read the points until 'endData' is read
    for (std::string line : dataLines) {
        // lemlib::infoSink()->debug("read raw line {}", stringToHex(line));
        if (line == "endData" || line == "endData\r") break;
        const std::vector<std::string> temp = readElementMagic(line, ", ");
        pointInput.push_back(temp); // parse line
    }

    return pointInput;
}

// TODO: add read/write stuff