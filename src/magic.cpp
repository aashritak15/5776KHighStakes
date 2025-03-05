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
    // Open two files for writing, truncating any existing content.
    fileO.open("/usd/autonomous.txt", std::ios::out | std::ios::trunc);
    fileOTwo.open("/usd/extra.txt", std::ios::out | std::ios::trunc);

    // Check if either file failed to open
    if (!fileO || !fileOTwo) {
        controller.set_text(0, 0, "failed to open both"); // Let the user know something went wrong
    } 
    else {
        controller.set_text(0, 0, "goated"); // Everything worked, so we send a W message
        active = true; // Mark that files are successfully opened
    }
}

void initInterrupt(int lastSection, int stopIndex) {
    std::cout << "DEBUG: initInterrupt() started\n"; // Debugging log to track execution

    // Open files for writing
    fileInterrupt.open("/usd/interruptAutonomous.txt", std::ios::out | std::ios::trunc);
    fileInterruptTwo.open("/usd/interruptExtra.txt", std::ios::out | std::ios::trunc);
    

    // Open original files for reading
    fileI.open("/usd/autonomous.txt");
    fileITwo.open("/usd/extra.txt");

    // Check if any file failed to open
    if (!fileInterrupt || !fileInterruptTwo || !fileI || !fileITwo) {
        controller.set_text(0, 0, "failed to open"); // Let the user know something went wrong
        active = false;
    } else {
        controller.set_text(0, 0, "open successful"); // Files opened successfully
        active = true;
    }

    std::string dataLine;

    // Copy `stopIndex` lines from `fileI` to `fileInterrupt`
    for (int i = 0; i < stopIndex; i++) { 
        std::getline(fileI, dataLine);
        fileInterrupt << dataLine << "\n";
        controller.set_text(0, 0, "copied fileI <3"); // Feedback for each line copied
    }

    // Copy `stopIndex` lines from `fileITwo` to `fileInterruptTwo`
    for (int j = 0; j < stopIndex; j++) { 
        std::getline(fileITwo, dataLine);
        fileInterruptTwo << dataLine << "\n";
        controller.set_text(0, 0, "copied fileITwo <3"); 
    }

    // Update section index
    section = lastSection + 1;

  
    fileInterrupt.flush();
    fileInterruptTwo.flush();

    // Close files properly
    fileI.close();
    if (fileITwo.is_open()) {
        fileITwo.close();
        pros::delay(50); // Small delay to avoid issues with file handling
    }

    controller.set_text(0, 0, "copied"); // Final confirmation message

    pros::delay(300); // Delay before moving on

    std::cout << "DEBUG: initInterrupt() finished, transitioning to opcontrol\n"; // Debugging log

    opcontrol(); // Move back into driver control 
}

void initDebug() {
    // Attempt to open the debug file for reading
    fileOThree.open("/usd/debug.txt");

    // Check if the file failed to open
    if (!fileOThree) {
        std::cout << "debug failed\n"; // Let the user know something went wrong
    } else {
        std::cout << "debug opened\n"; // Confirm successful opening
        active = true; 
    }
}

void closeO() {
    // Check if the A button on the controller is pressed
    // if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
    //     // Write final data to fileOTwo
    //     std::string dataLine = "0, 0, 0.000000, 0, 0, 0, STOPPED, -1\nendData";
    //     fileOTwo << dataLine;

    //     std::string dataLine2;

    //     // Append the chassis position data, rounded to 3 decimal places
    //     dataLine2.append(std::to_string((round(chassis.getPose().x * 1000)) / 1000) + ", ");
    //     dataLine2.append(std::to_string((round(chassis.getPose().y * 1000)) / 1000) + ", ");
    //     dataLine2.append(std::to_string((round(chassis.getPose().theta * 1000)) / 1000) + ", ");
    //     dataLine2.append("0\nendData");

    //     // Write position data to fileO
    //     fileO << dataLine2;

    //     // Ensure data is written to disk immediately
    //     fileO.flush();
    //     fileOTwo.flush();

    //     // Display a waiting message and pause
    //     controller.set_text(0, 0, "wait                          ");
    //     pros::delay(2500);

    //     // Close files properly
    //     if (fileO.is_open()) fileO.close();
    //     if (fileOTwo.is_open()) fileOTwo.close();

    //     pros::delay(100); // Small delay for stability

    //     active = false; 

    //     controller.set_text(0, 0, "file closed"); // Confirm file closure
    //     pros::delay(1000);

    //     controller.set_text(0, 0, "running filters"); // Notify user of next step

    //     filterAuton(); // Run filtering process
    // }
}

void closeOInterrupt() {
    // Check if the A button on the controller is pressed
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
        // Write final stop data to fileInterruptTwo
        std::string dataLine = "0, 0, 0.000000, 0, 0, 0, STOPPED, -1\nendData";
        fileInterruptTwo << dataLine;

        // Clear and reset dataLine before writing position data
        dataLine = "";
        dataLine.clear();
        pros::delay(200); // Small delay for stability

        // Append chassis position data, rounded to 3 decimal places
        dataLine.append(std::to_string((round(chassis.getPose().x * 1000)) / 1000) + ", ");
        dataLine.append(std::to_string((round(chassis.getPose().y * 1000)) / 1000) + ", ");
        dataLine.append(std::to_string((round(chassis.getPose().theta * 1000)) / 1000) + ", ");
        dataLine.append("0\nendData");

        // Write position data to fileInterrupt
        fileInterrupt << dataLine;

        
        fileInterrupt.flush();
        fileInterruptTwo.flush();

        // Display a waiting message and pause
        controller.set_text(0, 0, "wait                          ");
        pros::delay(2500);

        // Close the interrupt files properly
        fileInterrupt.close();
        fileInterruptTwo.close();

        active = false; // Mark system as inactive

        controller.set_text(0, 0, "file closed"); // Confirm file closure
    }
}

void writePose() {
    std::string dataLine = "";

    // Get motor voltages
    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage();

    // Calculate adjusted average voltage, rounded to 3 decimal places
    float adjusted = round((right + left) * 1000.0) / 1000.0 / 2;

    // Append chassis position data, all rounded to 3 decimal places
    dataLine.append(std::to_string((round(chassis.getPose().x * 1000)) / 1000) + ", ");
    dataLine.append(std::to_string((round(chassis.getPose().y * 1000)) / 1000) + ", ");
    dataLine.append(std::to_string((round(chassis.getPose().theta * 1000)) / 1000) + ", "); 
    dataLine.append(std::to_string(adjusted) + "\n");

    // Write pose data to file
    fileO << dataLine;

    // If writing fails and the system is active, notify the user
    if (!fileO && active) { 
        controller.set_text(0, 0, "write error"); 
    }
}

void writeAdditional() {
    //* Data mapping:
    //* 0 - intake, 1 - mogo, 2 - lb, 3 - doinker right, 4 - state, 5 - segment, 6 - doinker left

    std::string dataLine = "";

    // Get motor voltages
    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage();
    float total = left + right;

    // Detect button press for segmenting (resets on release)
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) { // TODO: change button back to X
        if (!buttonPressed) { 
            buttonPressed = true;
            section++; // Increment section when button is pressed
        }
    } else {
        buttonPressed = false; // Reset flag on button release
    }

    // Append system states to the data line
    dataLine.append(std::to_string(intakeState) + ", "); // 0: Intake state
    dataLine.append(std::to_string(clampState) + ", "); // 1: Mogo clamp state
    dataLine.append(std::to_string(globalTarget) + ", "); // 2: Lift/position target
    dataLine.append(std::to_string(doinkRightState) + ", "); // 3: Right doinker state
    dataLine.append(std::to_string(intakePistonState) + ", "); // 4: Intake piston state
    dataLine.append(std::to_string(doinkLeftState) + ", "); // 5: Left doinker state

    // Determine movement state based on motor voltages
    if (std::abs(total) < 600) { // TODO: Tune stop threshold
        // Detect turning based on individual motor values
        if (std::abs(right) > 800 && std::abs(left) > 800) { 
            if (right < 0) {
                dataLine.append("TURNING CW, "); // Clockwise turn
            } else if (right > 0) {
                dataLine.append("TURNING CCW, "); // Counterclockwise turn
            }
        } 
        // TODO: Reconsider subsystem detection for more accuracy


        else {
            dataLine.append("STOPPED, "); // Robot is not moving
        }
    } else {
        dataLine.append("GOING, "); // Robot is in motion
    }

    // Append current section number
    dataLine.append(std::to_string(section) + "\n");

    // Write data to file
    fileOTwo << dataLine;
}

void writeInterruptPose() {
    std::string dataLine = "";

    // Get motor voltages
    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage();

    // Calculate adjusted total voltage, rounded to 3 decimal places
    float adjusted = round((right + left) * 1000.0) / 1000.0;

    // Append chassis position data, all rounded to 3 decimal places
    dataLine.append(std::to_string((round(chassis.getPose().x * 1000)) / 1000) + ", ");
    dataLine.append(std::to_string((round(chassis.getPose().y * 1000)) / 1000) + ", ");
    dataLine.append(std::to_string((round(chassis.getPose().theta * 1000)) / 1000) + ", "); // Angle instead of speed
    dataLine.append(std::to_string(adjusted) + "\n"); // Append adjusted motor voltage

    // Write pose data to the interrupt file
    fileInterrupt << dataLine;

    // If writing fails and the system is active, notify the user
    if (!fileInterrupt && active) { 
        controller.set_text(0, 0, "write error"); 
    }
}

void writeInterruptAdditional() {
    //* Data mapping:
    //* 0 - intake, 1 - mogo, 2 - lb, 3 - doinker right, 4 - doinker left, 5 - intake piston state

    std::string dataLine = "";

    // Get motor voltages
    std::int32_t left = leftMotors.get_voltage();
    std::int32_t right = rightMotors.get_voltage();
    float total = left + right;

    // Detect button press for segmenting (resets on release)
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) { // TODO: Change button back if needed
        if (!buttonPressed) { 
            buttonPressed = true;
            section++; // Increment section when button is pressed
        }
    } else {
        buttonPressed = false; // Reset flag on button release
    }

    // Append system states to the data line
    dataLine.append(std::to_string(intakeState) + ", "); // 0: Intake state
    dataLine.append(std::to_string(clampState) + ", "); // 1: Mogo clamp state
    dataLine.append(std::to_string(globalTarget) + ", "); // 2: Lift/position target
    dataLine.append(std::to_string(doinkRightState) + ", "); // 3: Right doinker state
    dataLine.append(std::to_string(doinkLeftState) + ", "); // 4: Left doinker state
    dataLine.append(std::to_string(intakePistonState) + "\n"); // 5: Intake piston state

    // Determine movement state based on motor voltages
    if (std::abs(total) < 600) { // TODO: Tune stop threshold
        // Detect turning based on individual motor values
        if (std::abs(right) > 800 && std::abs(left) > 800) { 
            if (right < 0) {
                dataLine.append("TURNING CW, "); // Clockwise turn
            } else if (right > 0) {
                dataLine.append("TURNING CCW, "); // Counterclockwise turn
            }
        } 
        // TODO: Consider adding subsystem detection if needed

        else {
            dataLine.append("STOPPED, "); // Robot is not moving
        }
    } else {
        dataLine.append("GOING, "); // Robot is in motion
    }

    // Append current section number
    dataLine.append(std::to_string(section) + "\n");

    // Write data to interrupt file
    fileInterruptTwo << dataLine;
}

void reflect(bool x, bool y) {
    // Open output file for writing 
    reflector.open("/usd/reflected.txt", std::ios::out | std::ios::trunc);

    // Open input file for reading
    fileI.open("/usd/autonomous.txt");

    // Check if any file failed to open
    if (!reflector || !reflectorTwo || !fileI || !fileITwo) {
        controller.set_text(0, 0, "failed to open");
        std::cout << "failed to open";
        return;
    }

    std::string tempData;
    std::cout << "opened all\n";
    pros::delay(1000); // Small delay for stability

    // Read input file line by line
    while (std::getline(fileI, tempData)) {
        std::string dataLine;
        std::vector<std::string> pointData = readElementMagic(tempData, ", ");

        // Ensure correct data format (4 elements per line)
        if (pointData.size() != 4) {
            break;
        }

        // Process each data field
        for (int i = 0; i < 4; i++) {
            if (i == 0) { // X coordinate
                if (y) {
                    dataLine.append(std::to_string(std::stof(pointData.at(i)) * -1) + ", ");
                } else {
                    dataLine.append(pointData.at(i) + ", ");
                }
            } else if (i == 1) { // Y coordinate
                if (x) {
                    dataLine.append(std::to_string(std::stof(pointData.at(i)) * -1) + ", ");
                } else {
                    dataLine.append(pointData.at(i) + ", ");
                }
            } else if (i == 2) { // Heading
                if (x || y) { // Reflect heading if reflecting in either axis
                    dataLine.append(std::to_string(std::stof(pointData.at(i)) * -1) + ", ");
                } else { // No reflection
                    dataLine.append(pointData.at(i) + ", ");
                }
            } else if (i == 3) { 
                dataLine.append(pointData.at(i) + "\n");
            }

            std::cout << pointData.at(i) << "\n"; // Debugging output
        }

        // Write transformed data to reflector file
        reflector << dataLine;
        std::cout << dataLine << "\n"; // Debugging output

        // Check if writing failed
        if (!reflector) {
            std::cout << "reflector fail\n";
            return;
        }

        // Check if reading failed
        if (!fileI) {
            std::cout << "input fail\n";
            return;
        }

        tempData.clear(); // Clear temporary data storage
    }

    std::cout << "reflection finished\n";

    // Write end marker and close files
    reflector << "endData";
    reflector.flush();
    reflector.close();
    fileI.close();

    pros::delay(1000); // Small delay for stability
    std::cout << "closed\n";
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

// Removes isolated turns that are not followed or preceded by other turns
void removeIsolatedTurns(std::vector<std::vector<std::string>>& extra, std::vector<std::string>& autonomous) {
    controller.set_text(0, 0, "removing isolated turns");
    std::vector<std::vector<std::string>> cleaned_extra;
    std::vector<std::string> cleaned_autonomous;
    
    // Iterate through all the data points
    for (size_t i = 0; i < extra.size(); i++) {
        // Check if the current data point is a turn (CW or CCW)
        // and whether it is isolated (not preceded or followed by a turn)
        if ((extra[i][6] == "TURNING CW," || extra[i][6] == "TURNING CCW,") &&
            ((i == 0 || (extra[i - 1][6] != "TURNING CW," && extra[i - 1][6] != "TURNING CCW,")) &&
             (i == extra.size() - 1 || (extra[i + 1][6] != "TURNING CW," && extra[i + 1][6] != "TURNING CCW,")))) {
            continue; // Skip isolated turns
        }
        cleaned_extra.push_back(extra[i]);
        cleaned_autonomous.push_back(autonomous[i]);
    }

    // Update the original vectors with the cleaned ones
    extra = cleaned_extra;
    autonomous = cleaned_autonomous;
    controller.set_text(0, 0, "done removing isolated turns");
}

// Removes sequences of "STOPPED" data, limiting to a maximum of 20 consecutive stopped points
void stoppedSequences(std::vector<std::vector<std::string>>& extra, std::vector<std::string>& autonomous) {
    controller.set_text(0, 0, "removing stopped sequences");
    std::vector<std::vector<std::string>> cleaned_extra;
    std::vector<std::string> cleaned_autonomous;
    int stop_count = 0;

    // Iterate through all the data points
    for (size_t i = 0; i < extra.size(); i++) {
        // If we encounter a "STOPPED" state, increment stop_count
        if (extra[i][6].find("STOPPED") != std::string::npos) {
            stop_count++;
            if (stop_count > 20) continue; // Skip if stop_count exceeds 20
        } else {
            stop_count = 0; // Reset stop count if it's not a "STOPPED" state
        }
        cleaned_extra.push_back(extra[i]);
        cleaned_autonomous.push_back(autonomous[i]);
    }

    // Update the original vectors with the cleaned ones
    extra = cleaned_extra;
    autonomous = cleaned_autonomous;
    controller.set_text(0, 0, "done removing stopped sequences");
}

// Removes isolated "STOPPED" states that are not part of consecutive "STOPPED" states
void removeIsolatedStopped(std::vector<std::vector<std::string>>& extra, std::vector<std::string>& autonomous) {
    controller.set_text(0, 0, "removing isolated stops");
    std::vector<std::vector<std::string>> cleaned_extra;
    std::vector<std::string> cleaned_autonomous;

    // Iterate through all the data points
    for (size_t i = 0; i < extra.size(); i++) {
        // Check if the current point is a "STOPPED" and is isolated (not part of consecutive "STOPPED"s)
        if (extra[i][6] == "STOPPED," &&
            ((i == 0 || extra[i - 1][4] != "STOPPED,") && (i == extra.size() - 1 || extra[i + 1][4] != "STOPPED,"))) {
            continue; // Skip isolated stops
        }
        cleaned_extra.push_back(extra[i]);
        cleaned_autonomous.push_back(autonomous[i]);
    }

    // Update the original vectors with the cleaned ones
    extra = cleaned_extra;
    autonomous = cleaned_autonomous;
    controller.set_text(0, 0, "done removing isolated stops");
}

// Optimizes consecutive turns by keeping only the first and last turn in each sequence
void optimizeTurns(std::vector<std::vector<std::string>>& extra, std::vector<std::string>& autonomous) {
    controller.set_text(0, 0, "optimizing turns");
    std::vector<std::vector<std::string>> cleaned_extra;
    std::vector<std::string> cleaned_autonomous;
    size_t i = 0;

    // Iterate through the data points
    while (i < extra.size()) {
        // If the current point is a turn, process the entire sequence of turns
        if (extra[i][6] == "TURNING CW," || extra[i][6] == "TURNING CCW,") {
            size_t start = i;
            while (i < extra.size() && (extra[i][6] == "TURNING CW," || extra[i][6] == "TURNING CCW,")) { i++; }
            size_t end = i - 1;

            // Keep only the first and last turn in the sequence
            cleaned_extra.push_back(extra[start]);
            cleaned_extra.push_back(extra[end]);
            cleaned_autonomous.push_back(autonomous[start]);
            cleaned_autonomous.push_back(autonomous[end]);
        } else {
            cleaned_extra.push_back(extra[i]);
            cleaned_autonomous.push_back(autonomous[i]);
        }
        i++;
    }

    // Update the original vectors with the cleaned ones
    extra = cleaned_extra;
    autonomous = cleaned_autonomous;
    controller.set_text(0, 0, "done optimizing turns");
}



// Parses autonomous path data from an asset file and returns a vector of lemlib::Pose objects
std::vector<lemlib::Pose> getData(const asset& path) {
    std::vector<lemlib::Pose> robotPath;

    // Convert raw asset buffer into a string
    const std::string data(reinterpret_cast<char*>(path.buf), path.size);
    
    // Split data into lines using newline as the delimiter
    const std::vector<std::string> dataLines = readElementMagic(data, "\n");

    // Read and parse each line until "endData" is encountered
    for (std::string line : dataLines) {
        if (line == "endData" || line == "endData\r") break; // Stop reading when reaching the end marker

        // Parse individual elements from the line using ", " as a delimiter
        const std::vector<std::string> pointInput = readElementMagic(line, ", ");

        // Create a Pose object and populate it with parsed values
        lemlib::Pose pathPoint(0, 0);
        pathPoint.x = std::stof(pointInput.at(0)); // X coordinate
        pathPoint.y = std::stof(pointInput.at(1)); // Y coordinate
        pathPoint.theta = std::stof(pointInput.at(2)); // Heading (theta)

        robotPath.push_back(pathPoint); // Add the parsed point to the path vector
    }

    return robotPath;
}

// Parses subsystem data from an asset file and returns a vector of string vectors
std::vector<std::vector<std::string>> getSubData(const asset& sub) {
    // Convert raw asset buffer into a string
    const std::string data(reinterpret_cast<char*>(sub.buf), sub.size);

    // Split data into lines using newline as the delimiter
    const std::vector<std::string> dataLines = readElementMagic(data, "\n");

    std::vector<std::vector<std::string>> pointInput; // Store parsed subsystem data

    // Read and parse each line until "endData" is encountered
    for (std::string line : dataLines) {
        
        if (line == "endData" || line == "endData\r") break; // Stop reading when reaching the end marker

        // Parse individual elements from the line using ", " as a delimiter
        const std::vector<std::string> temp = readElementMagic(line, ", ");
        
        pointInput.push_back(temp); // Store parsed subsystem data
    }

    return pointInput;
}

