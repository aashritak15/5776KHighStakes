// The implementation below is mostly based off of
// the document written by Dawgma
// Here is a link to the original document
// https://www.chiefdelphi.com/uploads/default/original/3X/b/e/be0e06de00e07db66f97686505c3f4dde2e332dc.pdf

// k team on top tho.

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include "lemlib/logger/logger.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/util.hpp"
#include "globals.hpp"
#include "intake.hpp"
#include "magic.hpp"
#include "piston.hpp"
#include "ladybrown.hpp"

std::vector<lemlib::Pose> pathPoints;
std::vector<std::vector<std::string>> subValues;
std::vector<std::string> velocities;
int closestPoint = 0;
const float MIN_LOOKAHEAD = 5;
const float MAX_LOOKAHEAD = 10;

/**
 * @brief function that returns elements in a file line, separated by a delimeter
 *
 * @param input the raw string
 * @param delimeter string separating the elements in the line
 * @return std::vector<std::string> array of elements read from the file
 */
std::vector<std::string> readElement(const std::string& input, const std::string& delimiter) {
    std::string token;
    std::string substring = input;
    std::vector<std::string> output;
    size_t pos = 0;

    // main loop
    while ((pos = substring.find(delimiter)) != std::string::npos) { // while there are still delimiters in the string
        token = substring.substr(0, pos); // processed substring
        output.push_back(token);
        substring.erase(0, pos + delimiter.length()); // remove the read substring
    }

    output.push_back(substring); // add the last element to the returned string

    return output;
}

/**
 * @brief Convert a string to hex
 *
 * @param input the string to convert
 * @return std::string hexadecimal output
 */
std::string stringToHex(const std::string& input) {
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input) {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

/**
 * @brief Get a path from the sd card
 *
 * @param filePath The file to read from
 * @return std::vector<lemlib::Pose> vector of points on the path
 */

std::vector<std::string> getVelocities(const asset& path) {
    std::vector<std::string> robotVelocities;

    // format data from the asset
    const std::string data(reinterpret_cast<char*>(path.buf), path.size);
    const std::vector<std::string> dataLines = readElement(data, "\n");

    // read the points until 'endData' is read
    for (std::string line : dataLines) {
        if (line == "endData" || line == "endData\r") break;

        const std::vector<std::string> pointInput = readElement(line, ", "); // parse line

        robotVelocities.push_back(pointInput.at(3));
    }

    return robotVelocities;
}

/**
 * @brief find the closest point on the path to the robot
 *
 * @param pose the current pose of the robot
 * @param path the path to follow
 * @return int index to the closest point
 */
int findClosest(lemlib::Pose pose, int prevIndex) {
    int closestPoint;
    float closestDist = infinity();
    int maxIndex;

    if (prevIndex + 5 > pathPoints.size()) {
        maxIndex = pathPoints.size();
    } else {
        maxIndex = prevIndex + 5; // TODO: tune path skip tolerance
    }

    // loop through all path points
    for (int i = prevIndex; i < maxIndex; i++) {
        const float dist = pose.distance(pathPoints.at(i));
        if (dist < closestDist) { // new closest point
            closestDist = dist;
            closestPoint = i;
        }
    }

    return closestPoint;
}

/**
 * @brief Function that finds the intersection point between a circle and a line
 *
 * @param p1 start point of the line
 * @param p2 end point of the line
 * @param pos position of the robot
 * @param path the path to follow
 * @return float how far along the line the
 */
float circleIntersect(lemlib::Pose p1, lemlib::Pose p2, lemlib::Pose pose, float lookaheadDist) {
    // calculations
    // uses the quadratic formula to calculate intersection points
    lemlib::Pose d = p2 - p1;
    lemlib::Pose f = p1 - pose;
    float a = d * d;
    float b = 2 * (f * d);
    float c = (f * f) - lookaheadDist * lookaheadDist;
    float discriminant = (b * b) - (4 * a * c);

    // if a possible intersection was found
    if (discriminant >= 0) {
        discriminant = sqrt(discriminant);
        float t1 = (-b - discriminant) / (2 * a);
        float t2 = (-b + discriminant) / (2 * a);

        // prioritize further down the path
        if (t2 >= 0 && t2 <= 1) return t2;
        else if (t1 >= 0 && t1 <= 1) return t1;
    }

    // no intersection found
    return -1;
}

/**
 * @brief returns the lookahead point
 *
 * @param lastLookahead - the last lookahead point
 * @param pose - the current position of the robot
 * @param path - the path to follow
 * @param closest - the index of the point closest to the robot
 * @param lookaheadDist - the lookahead distance of the algorithm
 */
lemlib::Pose lookaheadPoint(lemlib::Pose lastLookahead, lemlib::Pose pose, int closest, float lookaheadDist) {
    // optimizations applied:
    // only consider xintersections that have an index greater than or equal to the point closest
    // to the robot
    // and intersections that have an index greater than or equal to the index of the last
    // lookahead point

    for (int i = closest; i < pathPoints.size() - 1; i++) { //*-1 is for endData
        lemlib::Pose lastPathPose = pathPoints.at(i);
        lemlib::Pose currentPathPose = pathPoints.at(i + 1);

        if (subValues.at(i)[6] == "STOPPED" || subValues.at(i)[6] == "TURNING CW" ||
            subValues.at(i)[6] == "TURNING CCW") { // TODO: does this make sense
            return pathPoints.at(i);
        }

        float t = circleIntersect(lastPathPose, currentPathPose, pose, lookaheadDist);

        if (t != -1) {
            lemlib::Pose lookahead = lastPathPose.lerp(currentPathPose, t);
            return lookahead;
        }
    }

    // robot deviated from path, use last lookahead point
    return lastLookahead;
}

/**
 * @brief Get the curvature of a circle that intersects the robot and the lookahead point
 *
 * @param pos the position of the robot
 * @param heading the heading of the robot
 * @param lookahead the lookahead point
 * @return float curvature
 */
float calculateCurvature(lemlib::Pose pose, float heading, lemlib::Pose lookahead) {
    // calculate whether the robot is on the left or right side of the circle
    float side = lemlib::sgn(std::sin(heading) * (lookahead.x - pose.x) - std::cos(heading) * (lookahead.y - pose.y));
    // calculate center point and radius
    float a = -std::tan(heading);
    float c = std::tan(heading) * pose.x - pose.y;
    float x = std::fabs(a * lookahead.x + lookahead.y + c) / std::sqrt((a * a) + 1);
    float d = std::hypot(lookahead.x - pose.x, lookahead.y - pose.y);

    std::cout << std::to_string(d) << "\n";

    if (d < 3) { // TODO: tune lookahead distance from exclusion tolerance
        return 0;
    }

    // return curvature
    return side * ((2 * x) / (d * d));
}

void updateSubsys() {
    intakeState = std::stoi(subValues.at(closestPoint)[0]);
    clampState = std::stoi(subValues.at(closestPoint)[1]);
    globalTarget = std::stod(subValues.at(closestPoint)[2]);
    doinkRightState = std::stoi(subValues.at(closestPoint)[3]);
    intakePistonState = std::stoi(subValues.at(closestPoint)[4]);
    doinkLeftState = std::stoi(subValues.at(closestPoint)[5]);

    // add left doinker in magic and here
}

bool doExclusions(std::string& dataLine) {
    // check for exclusions
    if (subValues.at(closestPoint)[6] == "STOPPED") {
        drivetrain.leftMotors->move(0);
        drivetrain.rightMotors->move(0);
        ladyBrown.move(0);

        pros::delay(50);

        dataLine.append("DELAYED\n\n");
        fileOThree << dataLine;
        fileOThree.flush();
        closestPoint++;
        return true;

    } else if (subValues.at(closestPoint)[6] == "TURNING CW" ||
               subValues.at(closestPoint)[6] == "TURNING CCW") { //*turn exclusion
        leftMotors.move(0);
        rightMotors.move(0);
        ladyBrown.move(0);

        if (subValues.at(closestPoint)[6] == "TURNING CW") {
            dataLine.append("TURN CLOCKWISE\n");
        } else {
            dataLine.append("TURN COUNTERCLOCKWISE\n");
        }

        int prevClosestPoint = closestPoint;

        closestPoint++; //*advance index forward one to begin the check
        while (subValues.at(closestPoint)[6] == "TURNING CW" || subValues.at(closestPoint)[6] == "TURNING CCW") {
            closestPoint++;
        }

        closestPoint++; //TODO: does this improve accuracy?

        dataLine.append("turn target index: " + std::to_string(closestPoint) + "\n");

        dataLine.append("target theta: " + std::to_string(pathPoints.at(closestPoint).theta) + "\n");

        if (subValues.at(prevClosestPoint)[6] == "TURNING CW") {
            dataLine.append("beginning theta: " + std::to_string(chassis.getPose().theta) + "\n");
            chassis.turnToHeading(pathPoints.at(closestPoint).theta, 2500,
                                  {.direction = AngularDirection::CW_CLOCKWISE, .maxSpeed = 80},
                                  false); // TODO: turn pid heading + max speed
            dataLine.append("ending theta: " + std::to_string(chassis.getPose().theta) +
                            "\n\n"); //* radians but i don't care anymore
        } else {
            dataLine.append("beginning theta: " + std::to_string(chassis.getPose().theta) + "\n");
            chassis.turnToHeading(pathPoints.at(closestPoint).theta, 2500,
                                  {.direction = AngularDirection::CCW_COUNTERCLOCKWISE, .maxSpeed = 80}, false);
            dataLine.append("ending theta: " + std::to_string(chassis.getPose().theta) + "\n");
        }

        closestPoint--;

        fileOThree << dataLine;
        fileOThree.flush();
        return true;
    }

    return false;
}

void doMultipliers(int segment, float& targetVel, std::string pathID) {
    if (pathID == "red mogo alliance" || pathID == "blue mogo alliance") {
        switch (std::stoi(subValues.at(closestPoint)[7])) {
            case 0: targetVel *= 2; break;
            case 1: targetVel *= 2; break;
            case 2: targetVel *= 2; break;
            case 3: targetVel *= 2; break;
            case 4: targetVel *= 2; break;
            case 5: targetVel *= 2; break;
            case 6: targetVel *= 2; break;
        }
    } else if (pathID == "red regional AWP" || pathID == "blue regional AWP") {
        switch (std::stoi(subValues.at(closestPoint)[7])) {
            case 0: targetVel *= 2; break;
            case 1: targetVel *= 2; break;
            case 2: targetVel *= 2; break;
            case 3: targetVel *= 2; break;
            case 4: targetVel *= 2; break;
            case 5: targetVel *= 2; break;
            case 6: targetVel *= 2; break;
        }
    } else if (pathID == "ASDF") {
        switch (std::stoi(subValues.at(closestPoint)[7])) {
            case 0: targetVel *= 2; break;
            case 1: targetVel *= 2; break;
            case 2: targetVel *= 2; break;
            case 3: targetVel *= 2; break;
            case 4: targetVel *= 2; break;
            case 5: targetVel *= 2; break;
            case 6: targetVel *= 2; break;
            case 7: targetVel *= 2; break;
            case 8: targetVel *= 2; break;
            case 9: targetVel *= 2; break;
            case 10: targetVel *= 2; break;
            case 11: targetVel *= 2; break;
            case 12: targetVel *= 2; break;
            case 13: targetVel *= 2; break;
        }
    } else if (pathID == "red solo wp") {
        switch (std::stoi(subValues.at(closestPoint)[7])) {
            case 0: targetVel *= 2; break;
            case 1: targetVel *= 2; break;
            case 2: targetVel *= 2; break;
            case 3: targetVel *= 2; break;
            case 4: targetVel *= 2; break;
            case 5: targetVel *= 2; break;
            case 6: targetVel *= 2; break;
            case 7: targetVel *= 1; break;
            case 8: targetVel *= 2; break;
            case 9: targetVel *= 2; break;
            case 10: targetVel *= 1; break;
            case 11: targetVel *= 1; break;
            case 12: targetVel *= 1; break;
            case 13: targetVel *= 1; break;
        }
    }
}

float findLookaheadCurvature(std::string& dataLine, lemlib::Pose lastLookahead, lemlib::Pose currentPose) {
    // adaptive lookahead
    float avgVel = round(((leftMotors.get_voltage() + rightMotors.get_voltage()) * 1000.0 / 2.0) / 1000.0);
    float pctVel = std::abs(avgVel / 12000);

    float lookaheadDist = MIN_LOOKAHEAD + ((MAX_LOOKAHEAD - MIN_LOOKAHEAD) * pctVel);

    dataLine.append("lookahead dist: " + std::to_string(lookaheadDist) + "\n"); // write lookahead

    // do lookahead
    lemlib::Pose lookaheadPose = lookaheadPoint(lastLookahead, currentPose, closestPoint, lookaheadDist);
    lastLookahead = lookaheadPose; //* update last lookahead position FOR DEVIATION FIXES

    dataLine.append("lookahead x: " + std::to_string(lookaheadPose.x) + "\n");
    dataLine.append("lookahead y: " + std::to_string(lookaheadPose.y) + "\n");

    float curvature = calculateCurvature(currentPose, M_PI / 2 - (currentPose.theta), lookaheadPose);

    dataLine.append("curvature: " + std::to_string(curvature) + "\n"); // write curvature

    return curvature;
}

void interrupt() {
    if (std::stoi(subValues.at(closestPoint)[7]) == 1) { // interrupt check (by segment)
        drivetrain.leftMotors->move(0);
        drivetrain.rightMotors->move(0);

        initInterrupt(stoi(subValues.at(closestPoint)[7]), closestPoint);

        opcontrol();

        return;
    }
}

void lemlib::Chassis::follow(const asset& path, const asset& sub, std::string pathID) {
    std::cout<<"following\n";
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    pathPoints = getData(path); // get list of path points
    subValues = getSubData(sub); // get subsystem values
    velocities = getVelocities(path); // get velocities

    Pose pose = this->getPose(true); // initialize all the things //*: THE TRUE IS WHERE THE RADIANS COME FROM
    Pose lookaheadPose(0, 0, 0);
    Pose lastLookahead = pathPoints.at(0);

    while (true) {
        std::cout<<"looping\n";
        // interrupt();

        // initialize debug dataline
        std::string dataLine = "";

        // get the current position of the robot
        pose = this->getPose(true);
        closestPoint = findClosest(pose, closestPoint); // find closest point

        // debug a new tick
        dataLine.append("NEW TICK\n");
        dataLine.append("target index: " + std::to_string(closestPoint) + "\n");

        // path termination check
        if (subValues.at(closestPoint)[7] == "-1") {
            drivetrain.leftMotors->move(0);
            drivetrain.rightMotors->move(0);
            dataLine.append("PATH FINISHED");

            fileOThree << dataLine;
            fileOThree.flush();
            fileOThree.close();

            return;
        }

        // update all subsystems
        updateSubsys();

        // exclusions
        bool skip = doExclusions(dataLine);
        if (skip) { continue; }

        // debug non-exclusion-relevant information
        dataLine.append("current x: " + std::to_string(pose.x) + "\n");
        dataLine.append("current y: " + std::to_string(pose.y) + "\n");
        dataLine.append("current theta RAD: " + std::to_string(pose.theta) + "\n");
        dataLine.append("current theta DEG: " + std::to_string(pose.theta * 180 / M_PI) + "\n");
        dataLine.append("target x: " + std::to_string(pathPoints.at(closestPoint).x) + "\n");
        dataLine.append("target y: " + std::to_string(pathPoints.at(closestPoint).y) + "\n");

        // find target avg velocity
        float targetVel = std::stof(velocities.at(closestPoint));
        dataLine.append("target vel: " + std::to_string(targetVel) + "\n"); // write target vel

        // apply rerun multipliers
        doMultipliers(std::stoi(subValues.at(closestPoint)[7]), targetVel, pathID);

        // do all pure pursuit calculations
        float curvature = findLookaheadCurvature(dataLine, lastLookahead, pose);

        float targetLeftVel = targetVel * (2 + curvature * drivetrain.trackWidth) / 2;
        float targetRightVel = targetVel * (2 - curvature * drivetrain.trackWidth) / 2;

        // fallback exclusion
        if ((std::abs(targetLeftVel) < 600) && (std::abs(targetRightVel) < 600)) {
            dataLine.append("VEL < 600\n\n");
            leftMotors.move_velocity(0);
            rightMotors.move_velocity(0);
            ladyBrown.move(0);

            pros::delay(50); //*change to tick speed always
            closestPoint++;
            continue;
        }

        // ratio the speeds to respect the max speed
        float ratio = std::max(std::fabs(targetLeftVel), std::fabs(targetRightVel)) / 12000;
        if (ratio > 1) {
            targetLeftVel /= ratio;
            targetRightVel /= ratio;
        }

        // write velocities
        dataLine.append("current velocities: " + std::to_string(leftMotors.get_voltage()) + " " +
                        std::to_string(rightMotors.get_voltage()) + "\n");
        dataLine.append("target vels: " + std::to_string(targetLeftVel) + " " + std::to_string(targetRightVel) +
                        "\n\n");

        // send velocity
        leftMotors.move_voltage(targetLeftVel);
        rightMotors.move_voltage(targetRightVel);

        // write to debug
        fileOThree << dataLine;
        fileOThree.flush();

        pros::delay(10);
    }
}