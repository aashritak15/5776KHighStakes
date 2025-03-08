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
const float MIN_LOOKAHEAD = 5; //TODO: find good lookaheads
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
    int closestPoint; // Index of the closest point
    float closestDist = infinity(); // Initialize with the maximum possible distance
    int maxIndex; // Upper limit for searching

    // Set maxIndex to limit how far ahead we search for the closest point
    if (prevIndex + 20 > pathPoints.size() - 2) { // -1 for "endData", -1 because it's an index
        maxIndex = pathPoints.size() - 2;
    } else {
        maxIndex = prevIndex + 20; // TODO: Tune this value to adjust path skipping tolerance
    }

    // Loop through path points within the allowed range
    for (int i = prevIndex; i < maxIndex; i++) {
        const float dist = pose.distance(pathPoints.at(i)); // Calculate distance to the current path point

        // Debugging output (uncomment if needed)
        // std::cout << i << ": " << dist << "\n";

        // If this point is closer than the previous closest, update closest point
        if (dist < closestDist) { 
            closestDist = dist;
            closestPoint = i;
        }
    }

    return closestPoint; // Return the index of the closest path point
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
    // Vector calculations for the quadratic formula
    // d = direction vector of the path segment
    lemlib::Pose d = p2 - p1;

    // f = vector from the pose (robot position) to the start of the segment
    lemlib::Pose f = p1 - pose;

    // Quadratic formula coefficients (standard form: at^2 + bt + c = 0)
    float a = d * d;  // Squared magnitude of d
    float b = 2 * (f * d);  // 2 * dot(f, d)
    float c = (f * f) - (lookaheadDist * lookaheadDist);  // Difference in squared distances

    // Compute the discriminant to determine if an intersection exists
    float discriminant = (b * b) - (4 * a * c);

    // If there is a valid intersection
    if (discriminant >= 0) {
        discriminant = sqrt(discriminant);

        // Compute the two possible intersection points using the quadratic formula
        float t1 = (-b - discriminant) / (2 * a);
        float t2 = (-b + discriminant) / (2 * a);

        // Prioritize the intersection point further along the path
        if (t2 >= 0 && t2 <= 1) return t2;  // If t2 is within bounds, return it
        else if (t1 >= 0 && t1 <= 1) return t1;  // Otherwise, check t1
    }

    // No valid intersection found
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
    // Optimization applied:
    // - Only consider intersections with segments at or beyond the closest point to the robot
    // - Ensure intersections are at or beyond the last lookahead point

    for (int i = closest; i < pathPoints.size() - 1; i++) { // *-1 accounts for "endData"
        lemlib::Pose lastPathPose = pathPoints.at(i);
        lemlib::Pose currentPathPose = pathPoints.at(i + 1);

        // Check if the robot is in a stopped or turning state
        if (subValues.at(i)[6] == "STOPPED" || subValues.at(i)[6] == "TURNING CW" ||
            subValues.at(i)[6] == "TURNING CCW" || subValues.at(i)[7] == "-1") { // TODO: Confirm if this check is valid
            return pathPoints.at(i); // If stopped/turning, return the current path point
        }

        // Find intersection of lookahead circle with the current path segment
        float t = circleIntersect(lastPathPose, currentPathPose, pose, lookaheadDist);

        if (t != -1) { // If a valid intersection was found
            lemlib::Pose lookahead = lastPathPose.lerp(currentPathPose, t); // Interpolate to find the exact point
            return lookahead;
        }
    }

    // If no valid lookahead point is found, return the last known lookahead point
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
    // determine if the lookahead point is to the left or right of the robot
    float side = lemlib::sgn(std::sin(heading) * (lookahead.x - pose.x) - std::cos(heading) * (lookahead.y - pose.y));

    // calculate the perpendicular distance from the bot to the lookahead point
    float a = -std::tan(heading); // slope of the line perpendicular to the robot's heading
    float c = std::tan(heading) * pose.x - pose.y; // line equation offset
    float x = std::fabs(a * lookahead.x + lookahead.y + c) / std::sqrt((a * a) + 1); // Distance formula

    // compute the distance between the robot and the lookahead point
    float d = std::hypot(lookahead.x - pose.x, lookahead.y - pose.y);

    // ignore very close lookahead points to prevent erratic movements
    if (d < 5) { // TODO: Tune lookahead distance exclusion tolerance
        return 0;
    }

    // return the signed curvature value (higher curvature means a sharper turn)
    return side * ((2 * x) / (d * d));
}

/**
 * @brief Update all subsystem states for asynchronous subsystem tasks
 */
void updateSubsys() {
    // Parse subsystem states from the closest point's recorded values
    intakeState = std::stoi(subValues.at(closestPoint)[0]);       // Intake state
    clampState = std::stoi(subValues.at(closestPoint)[1]);       // Mogo clamp state
    globalTarget = std::stod(subValues.at(closestPoint)[2]);     // Lift/position target
    doinkRightState = std::stoi(subValues.at(closestPoint)[3]);  // Right doinker state
    intakePistonState = std::stoi(subValues.at(closestPoint)[4]);// Intake piston state
    doinkLeftState = std::stoi(subValues.at(closestPoint)[5]);   // Left doinker state
}

/**
 * @brief Check for special exclusions

 * @param dataLine reference to debug string
 */
bool doExclusions(std::string& dataLine) {
    // check if the robot needs to stop  
    if (subValues.at(closestPoint)[6] == "STOPPED") {
        drivetrain.leftMotors->move(0);
        drivetrain.rightMotors->move(0);

        pros::delay(50); // small pause before moving on  

        dataLine.append("delayed\n\n");
        fileOThree << dataLine;
        fileOThree.flush();

        closestPoint++; // go to the next point  
        return true; // exclusion handled  
    }

    // check if the robot needs to turn  
    else if (subValues.at(closestPoint)[6] == "TURNING CW" ||
             subValues.at(closestPoint)[6] == "TURNING CCW") {
        leftMotors.move_voltage(0);
        rightMotors.move_voltage(0);

        pros::delay(100); // short delay before turning  

        // add turn direction to log  
        if (subValues.at(closestPoint)[6] == "TURNING CW") {
            dataLine.append("turn clockwise\n");
        } else {
            dataLine.append("turn counterclockwise\n");
        }

        int prevClosestPoint = closestPoint; // save the current point  

        // skip over all turn points  
        closestPoint++;
        while (subValues.at(closestPoint)[6] == "TURNING CW" || subValues.at(closestPoint)[6] == "TURNING CCW") {
            closestPoint++;
        }

        // log turn details  
        dataLine.append("turn target index: " + std::to_string(closestPoint) + "\n");
        dataLine.append("target theta: " + std::to_string(pathPoints.at(closestPoint).theta) + "\n");

        // decide which way to turn  
        if (subValues.at(prevClosestPoint)[6] == "TURNING CW") {
            dataLine.append("beginning theta: " + std::to_string(chassis.getPose().theta) + "\n");

            chassis.turnToHeading(pathPoints.at(closestPoint).theta, 2500,
                                  {.direction = AngularDirection::CW_CLOCKWISE, .maxSpeed = 80}, 
                                  false); // todo: adjust max speed  

            dataLine.append("ending theta: " + std::to_string(chassis.getPose().theta) + "\n\n"); 
        } else {
            dataLine.append("beginning theta: " + std::to_string(chassis.getPose().theta) + "\n");

            chassis.turnToHeading(pathPoints.at(closestPoint).theta, 2500,
                                  {.direction = AngularDirection::CCW_COUNTERCLOCKWISE, .maxSpeed = 80}, 
                                  false);

            dataLine.append("ending theta: " + std::to_string(chassis.getPose().theta) + "\n");
        }

        fileOThree << dataLine;
        fileOThree.flush();

        pros::delay(100); // short pause before moving on  
        return true; // exclusion handled  
    }

    return false; // no exclusion triggered  
}

/**
 * @brief Multiply target velocity based on segment

 * @param segment current segment
 * @param targetVel reference to current tick's target velocity
 * @param pathID identifier to chose correct multiplier
 */
void doMultipliers(int segment, float& targetVel, std::string pathID) {
    if (pathID == "ringside") {
        // check segment type and adjust velocity  
        switch (std::stoi(subValues.at(closestPoint)[7])) {
            case 0: targetVel *= 1; break; // stop  
            case 1: targetVel *= 3; break;  
            case 2: targetVel *= 1; break; // turn  
            case 3: targetVel *= 3; break;  
            case 4: targetVel *= 3; break;  
            case 5: targetVel *= 3; break;  
            case 6: targetVel *= 2.25; break;  
            case 7: targetVel *= 2.5; break;  
            case 8: targetVel *= 2.5; break;  
            case 9: targetVel *= 2.5; break;  
        }
    } else if (pathID == "skills") {
        // apply a flat multiplier for all cases  
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
        }
    }
}

/**
 * @brief Finds the curvature needed to follow the path based on a dynamic lookahead distance 

 * @param dataLine reference to debug string
 * @param lastLookahead last lookahead point
 * @param currentPose current lookahead point
 */ 
float findLookaheadCurvature(std::string& dataLine, lemlib::Pose lastLookahead, lemlib::Pose currentPose) {
    // calculate average motor voltage  
    float avgVel = round(((leftMotors.get_voltage() + rightMotors.get_voltage()) * 1000.0 / 2.0) / 1000.0);
    float pctVel = std::abs(avgVel / 12000); // get velocity as a percentage of max  

    // adjust lookahead distance based on speed  
    float lookaheadDist = MIN_LOOKAHEAD + ((MAX_LOOKAHEAD - MIN_LOOKAHEAD) * pctVel);

    dataLine.append("lookahead dist: " + std::to_string(lookaheadDist) + "\n"); // log lookahead  

    // find the next lookahead point  
    lemlib::Pose lookaheadPose = lookaheadPoint(lastLookahead, currentPose, closestPoint, lookaheadDist);
    lastLookahead = lookaheadPose; // update last lookahead position for deviation fixes  

    // log lookahead point details  
    dataLine.append("lookahead x: " + std::to_string(lookaheadPose.x) + "\n");
    dataLine.append("lookahead y: " + std::to_string(lookaheadPose.y) + "\n");

    // calculate curvature for the turn  
    float curvature = calculateCurvature(currentPose, M_PI / 2 - (currentPose.theta), lookaheadPose);

    dataLine.append("curvature: " + std::to_string(curvature) + "\n"); // log curvature  

    return curvature;
}
void interrupt() {
    if (std::stoi(subValues.at(closestPoint)[7]) == 1) { // interrupt check (by segment)  
        drivetrain.leftMotors->move(0);
        drivetrain.rightMotors->move(0);

        // start the interrupt process  
        initInterrupt(stoi(subValues.at(closestPoint)[7]), closestPoint);


        //switch to driver control
        opcontrol();

        return;
    }
}

/**
 * @brief Follow a VEX V5 KiwiRun path using Pure Pursuit

 * @param path path file
 * @param sub sub-information file
 * @param pathID unique path identifier
 */ 
void lemlib::Chassis::follow(const asset& path, const asset& sub, std::string pathID) {
    std::cout << "following\n"; // start follower to console

    initDebug(); // start debug logging

    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE); 

    pathPoints = getData(path); // load the list of path points
    subValues = getSubData(sub); // load subsystem states
    velocities = getVelocities(path); // load recorded velocities
    std::cout << "data gotten\n";

    Pose pose = this->getPose(true); // get current robot position (true = radians)
    Pose lookaheadPose(0, 0, 0); // lookahead position for pure pursuit
    Pose lastLookahead = pathPoints.at(0); // initialize lookahead point
    std::cout << "initialized\n";

    int killCount = 1; // counter to prevent getting stuck
    int prevClosestPoint = 0; // track previous closest point

    while (true) {
        std::cout << "looping\n";
     

        std::string dataLine = ""; // debug string for logging

        pose = this->getPose(true); // update the robot's current position
        closestPoint = findClosest(pose, closestPoint); // find the closest recorded point on the path

        dataLine.append("NEW TICK\n");
        dataLine.append("target index: " + std::to_string(closestPoint) + "\n");

        // check if the path is finished
        if (subValues.at(closestPoint)[7] == "6" || prevClosestPoint > closestPoint) { 
            drivetrain.leftMotors->move(0);
            drivetrain.rightMotors->move(0);
            dataLine.append("PATH FINISHED");

            fileOThree << dataLine;
            fileOThree.flush();
            fileOThree.close();

            controller.set_text(0, 0, "PATH FINISHED!");

            return;
        }

        dataLine.append("kill count: " + std::to_string(killCount) + "\n");

        updateSubsys(); // update subsystem states

        // handle any exceptions (stops, turns, delays)
        bool skip = doExclusions(dataLine);
        if (skip) { continue; } // skip to next loop iteration if an exclusion is triggered

        // prevent the bot from getting stuck in the same position
        if (std::stoi(subValues.at(closestPoint)[7]) == prevClosestPoint) {
            killCount++;
            if (killCount == 8) { 
                dataLine.append("KILL TIMERED OUT\n\n");
                closestPoint++;
                prevClosestPoint = closestPoint;

                fileOThree << dataLine;
                fileOThree.flush();
                
                killCount = 1;
                continue;
            }
        } else {
            killCount = 1; // reset kill count if progress is being made
        }

        // log position data for debugging
        dataLine.append("current x: " + std::to_string(pose.x) + "\n");
        dataLine.append("current y: " + std::to_string(pose.y) + "\n");
        dataLine.append("current theta RAD: " + std::to_string(pose.theta) + "\n");
        dataLine.append("current theta DEG: " + std::to_string(pose.theta * 180 / M_PI) + "\n");
        dataLine.append("closest path x: " + std::to_string(pathPoints.at(closestPoint).x) + "\n");
        dataLine.append("closest path y: " + std::to_string(pathPoints.at(closestPoint).y) + "\n");

        // get the target velocity for the next movement
        float targetVel = std::stof(velocities.at(closestPoint));
        dataLine.append("target vel: " + std::to_string(targetVel) + "\n");

        // apply multipliers based on kiwirun segment data
        doMultipliers(std::stoi(subValues.at(closestPoint)[7]), targetVel, pathID);

        // calculate curvature for pure pursuit tracking
        float curvature = findLookaheadCurvature(dataLine, lastLookahead, pose);

        // calculate target left and right wheel speeds based on curvature
        float targetLeftVel = targetVel * (2 + curvature * drivetrain.trackWidth) / 2;
        float targetRightVel = targetVel * (2 - curvature * drivetrain.trackWidth) / 2;

        // stop moving if velocity is too low
        if ((std::abs(targetLeftVel) < 600) && (std::abs(targetRightVel) < 600)) {
            dataLine.append("VEL < 600\n\n");
            leftMotors.move_velocity(0);
            rightMotors.move_velocity(0);
            ladyBrown.move(0);

            pros::delay(50); // small delay before continuing
            closestPoint++;
            continue;
        }

        // scale speeds down if they exceed max motor voltage (12000)
        float ratio = std::max(std::fabs(targetLeftVel), std::fabs(targetRightVel)) / 12000;
        if (ratio > 1) {
            targetLeftVel /= ratio;
            targetRightVel /= ratio;
        }

        // log velocity data
        dataLine.append("current velocities: " + std::to_string(leftMotors.get_voltage()) + " " +
                        std::to_string(rightMotors.get_voltage()) + "\n");
        dataLine.append("target vels: " + std::to_string(targetLeftVel) + " " + std::to_string(targetRightVel) +
                        "\n\n");
 
        // send velocity commands to motors
        leftMotors.move_voltage(targetLeftVel);
        rightMotors.move_voltage(targetRightVel);

        prevClosestPoint = closestPoint; // update previous closest point

        // write debug data to file
        fileOThree << dataLine;
        fileOThree.flush();

        pros::delay(10); // small delay to prevent overloading
    }
}
