// The implementation below is mostly based off of
// the document written by Dawgma
// Here is a link to the original document
// https://www.chiefdelphi.com/uploads/default/original/3X/b/e/be0e06de00e07db66f97686505c3f4dde2e332dc.pdf

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include "pros/misc.hpp"
#include "lemlib/logger/logger.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/util.hpp"
#include "globals.hpp"
#include "intake.hpp"
#include "magic.hpp"
#include "piston.hpp"
#include "ladybrown.hpp"

/**
 * @brief function that returns elements in a file line, separated by a delimeter
 *
 * @param input the raw string
 * @param delimeter string separating the elements in the line
 * @return std::vector<std::string> array of elements read from the file
 */
std::vector<std::string> readElement(const std::string& input, const std::string& delimiter) {
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

    // for(int i=0; i<output.size(); i++){
    //     std::cout<<output[i]<<"\n";
    // }
    // std::cout<<output.size()<<"\n";

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
std::vector<lemlib::Pose> getData(const asset& path) {    
    std::vector<lemlib::Pose> robotPath;
    // format data from the asset
    const std::string data(reinterpret_cast<char*>(path.buf), path.size);
    const std::vector<std::string> dataLines = readElement(data, "\n");

    // read the points until 'endData' is read
    for (std::string line : dataLines) {
        lemlib::infoSink()->debug("read raw line {}", stringToHex(line));
        
        if (line == "endData" || line == "endData\r") break;

        const std::vector<std::string> pointInput = readElement(line, ", "); // parse line

        lemlib::Pose pathPoint(0, 0);
        pathPoint.x = std::stof(pointInput.at(0)); // x position
        pathPoint.y = std::stof(pointInput.at(1)); // y position
        pathPoint.theta = std::stof(pointInput.at(2)); // ANGLE???? what a jumpscare
        robotPath.push_back(pathPoint); // save data
        // lemlib::infoSink()->debug("read point {}", pathPoint);

        // std::cout<<"x: "<<pathPoint.x<<"\n"; //* This works!!
        // std::cout<<"y: "<<pathPoint.y<<"\n";
        // std::cout<<"vel: "<<pathPoint.theta<<"\n\n";
    }

    return robotPath;
}

std::vector<std::string> getVelocities(const asset& path) {
    std::vector<std::string> robotVelocities;

    const std::string data(reinterpret_cast<char*>(path.buf), path.size);
    const std::vector<std::string> dataLines = readElement(data, "\n");

    for (std::string line : dataLines) {
        
        if (line == "endData" || line == "endData\r") break;

        const std::vector<std::string> pointInput = readElement(line, ", "); // parse line

        robotVelocities.push_back(pointInput.at(3));
    }

    return robotVelocities;
}

std::vector<std::vector<std::string>> getSubData(const asset& sub) {
    // format data from the asset
    const std::string data(reinterpret_cast<char*>(sub.buf), sub.size);
    const std::vector<std::string> dataLines = readElement(data, "\n");
    std::vector<std::vector<std::string>> pointInput;

    // read the points until 'endData' is read
    for (std::string line : dataLines) {
        // lemlib::infoSink()->debug("read raw line {}", stringToHex(line));
        if (line == "endData" || line == "endData\r") break;
        const std::vector<std::string> temp = readElement(line, ", ");
        pointInput.push_back(temp); // parse line

    }

    return pointInput;
}

/**
 * @brief find the closest point on the path to the robot
 *
 * @param pose the current pose of the robot
 * @param path the path to follow
 * @return int index to the closest point
 */
int findClosest(lemlib::Pose pose, std::vector<lemlib::Pose> path, int prevIndex) {
    int closestPoint;
    float closestDist = infinity();
    int maxIndex;

    if (prevIndex + 5 > path.size()) {
        maxIndex = path.size();
    } else {
        maxIndex = prevIndex + 5; //TODO: tune path skip tolerance
    }

    // loop through all path points
    for (int i = prevIndex; i < maxIndex; i++) { 
        const float dist = pose.distance(path.at(i));
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
lemlib::Pose lookaheadPoint(lemlib::Pose lastLookahead, lemlib::Pose pose, std::vector<lemlib::Pose> path, std::vector<std::vector<std::string>> subValues, int closest, float lookaheadDist) {
    // optimizations applied:
    // only consider xintersections that have an index greater than or equal to the point closest
    // to the robot
    // and intersections that have an index greater than or equal to the index of the last
    // lookahead point

    for (int i = closest; i < path.size() - 1; i++){ //*-1 is for endData
        lemlib::Pose lastPathPose = path.at(i);
        lemlib::Pose currentPathPose = path.at(i + 1);

        if(subValues.at(i)[4] == "STOPPED" || subValues.at(i)[4] == "TURNING CW" || subValues.at(i)[4] == "TURNING CCW") { //TODO: does this make sense
            return path.at(i);
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
float findLookaheadCurvature(lemlib::Pose pose, float heading, lemlib::Pose lookahead) {
    // calculate whether the robot is on the left or right side of the circle
    float side = lemlib::sgn(std::sin(heading) * (lookahead.x - pose.x) - std::cos(heading) * (lookahead.y - pose.y));
    // calculate center point and radius
    float a = -std::tan(heading);
    float c = std::tan(heading) * pose.x - pose.y;
    float x = std::fabs(a * lookahead.x + lookahead.y + c) / std::sqrt((a * a) + 1);
    float d = std::hypot(lookahead.x - pose.x, lookahead.y - pose.y);

    if (d < 3) { //TODO: tune lookahead distance from exclusion tolerance
        return 0;
    }

    // return curvature
    return side * ((2 * x) / (d * d));
}

void lemlib::Chassis::follow(const asset& path, const asset& sub, std::string pathID) {

    std::vector<lemlib::Pose> pathPoints = getData(path); // get list of path points
    std::vector<std::vector<std::string>> subValues = getSubData(sub); //get subsystem values
    std::vector<std::string> velocities = getVelocities(path); //get velocities

    Pose pose = this->getPose(true); //initialize all the things //*: THE TRUE IS WHERE THE RADIANS COME FROM
    Pose lookaheadPose(0, 0, 0); 
    Pose lastLookahead = pathPoints.at(0);

    float curvature;
    float lookaheadDist;

    float targetVel;
    float targetLeftVel;
    float targetRightVel;
    float avgVel;
    float pctVel;

    float ratio;

    int closestPoint = 0;

    float minLookahead = 5; //TODO: tune min/max lookahead
    float maxLookahead = 10;

    ladyBrown.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

    this->requestMotionStart();

    while (true) {
        // if(subValues.at(closestPoint)[5] == std::to_string(5)) { //interrupt check (by segment)
        //     drivetrain.leftMotors->move(0);
        //     drivetrain.rightMotors->move(0);

        //     initInterrupt(stoi(subValues.at(closestPoint)[5]), closestPoint);

        //     interruptLoop();
        // }

        // autonomous and extra files NEED to be same as the path you want to interrupt
        // if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)) { //interrupt check (by button) //TODO: swap button
        //     controller.set_text(0,0,"interrupt");
        //     drivetrain.leftMotors->move(0);
        //     drivetrain.rightMotors->move(0);

        //     initInterrupt(stoi(subValues.at(closestPoint)[5]), closestPoint);

        //     interruptLoop();
        // }
        
        std::string dataLine = ""; //initialize debug dataline

        // get the current position of the robot
        pose = this->getPose(true); //*this is where radians come from

        closestPoint = findClosest(pose, pathPoints, closestPoint); //find closest point 

        // line write: tick and index
        dataLine.append("NEW TICK\n");
        dataLine.append("target index: " + std::to_string(closestPoint) + "\n");

        // path termination check
        if (subValues.at(closestPoint)[5] == "-1") {
            drivetrain.leftMotors->move(0);
            drivetrain.rightMotors->move(0);
            dataLine.append("PATH FINISHED");

            fileOThree << dataLine;
            fileOThree.flush();
            fileOThree.close();

            this->endMotion();

            return;
        }

        //update all subsystems
        if (subValues.at(closestPoint)[0] == "0") {intake.move_voltage(0);} 
        else if (subValues.at(closestPoint)[0] == "1") {intake.move_voltage(12000);}
        else if (subValues.at(closestPoint)[0] == "2") {intake.move_voltage(-12000);}

        if (subValues.at(closestPoint)[1] == "0") {mogoClamp.set_value(false);} 
        else if (subValues.at(closestPoint)[1] == "1") {mogoClamp.set_value(true);}        

        globalTarget = stod(subValues.at(closestPoint)[2]);

        if (subValues.at(closestPoint)[3] == "0") {doink.set_value(false);} 
        else if (subValues.at(closestPoint)[3] == "1") {doink.set_value(true);} 

        //check for exclusions
        if (subValues.at(closestPoint)[4] == "STOPPED") { //*stop exclusion
            drivetrain.leftMotors->move(0);                 
            drivetrain.rightMotors->move(0);
            ladyBrown.move(0);

            pros::delay(50);//*CHANGE TO TICK SPEED

            dataLine.append("DELAYED\n\n");
            fileOThree<<dataLine;
            fileOThree.flush();
            closestPoint++;
            continue;

        } else if(subValues.at(closestPoint)[4] == "TURNING CW" || subValues.at(closestPoint)[4] == "TURNING CCW" ) { //*turn exclusion
            leftMotors.move(0);
            rightMotors.move(0);
            ladyBrown.move(0);
            this->endMotion();

            if(subValues.at(closestPoint)[4] == "TURNING CW") {
                dataLine.append("TURN CLOCKWISE\n");
            } else {
                dataLine.append("TURN COUNTERCLOCKWISE\n");
            }

            int prevClosestPoint = closestPoint;

            closestPoint++; //*advance index forward one to begin the check 
            while(subValues.at(closestPoint)[4] == "TURNING CW" || subValues.at(closestPoint)[4] == "TURNING CCW") {closestPoint++;}
            closestPoint++; //*go forward one packet past the turn to account for angle imperfections

            dataLine.append("turn target index: " + std::to_string(closestPoint) + "\n");

            dataLine.append("target theta: " + std::to_string(pathPoints.at(closestPoint).theta)+"\n");

            if (subValues.at(prevClosestPoint)[4] == "TURNING CW") {
                dataLine.append("beginning theta: " + std::to_string(chassis.getPose().theta) + "\n");
                chassis.turnToHeading(pathPoints.at(closestPoint).theta, 2500, {.direction = AngularDirection::CW_CLOCKWISE, .maxSpeed = 80}, false); //TODO: turn pid heading + max speed
                dataLine.append("ending theta: " + std::to_string(chassis.getPose().theta) + "\n\n"); //* radians but i don't care anymore
            } else {
                dataLine.append("beginning theta: " + std::to_string(chassis.getPose().theta) + "\n");
                chassis.turnToHeading(pathPoints.at(closestPoint).theta, 2500, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE, .maxSpeed = 80}, false);
                dataLine.append("ending theta: " + std::to_string(chassis.getPose().theta) + "\n");
            }

            // chassis.setPose(pathPoints[closestPoint].x, pathPoints[closestPoint].y, pathPoints[closestPoint].theta); // temp reset position because bot being fat ///TODO: we hate this forever and ever and evere and eva sdlkfj;asjf;klaew;isodjkl
            
            dataLine.append("current x: " + std::to_string(this->getPose().x) + "\n");
            dataLine.append("current y: " + std::to_string(this->getPose().y) + "\n\n");

            fileOThree<<dataLine;
            fileOThree.flush();

            closestPoint++;

            this->requestMotionStart();

            continue;
        }
        
        //line write: coordinates and target
        dataLine.append("current x: " + std::to_string(pose.x) + "\n");
        dataLine.append("current y: " + std::to_string(pose.y) + "\n");
        dataLine.append("current theta RAD: " + std::to_string(pose.theta) + "\n");
        dataLine.append("current theta DEG: " + std::to_string(pose.theta * 180 / M_PI) + "\n");
        dataLine.append("target x: " + std::to_string(pathPoints.at(closestPoint).x) + "\n");
        dataLine.append("target y: " + std::to_string(pathPoints.at(closestPoint).y) + "\n");

        // find the lookahead point

        // adaptive lookahead
        avgVel = round(((leftMotors.get_voltage() + rightMotors.get_voltage()) * 1000.0 / 2.0) / 1000.0);
        pctVel = std::abs(avgVel / 12000);

        lookaheadDist = minLookahead + ((maxLookahead - minLookahead) * pctVel);

        dataLine.append("lookahead dist: " + std::to_string(lookaheadDist) + "\n"); //write lookahead

        //do lookahead
        lookaheadPose = lookaheadPoint(lastLookahead, pose, pathPoints, subValues, closestPoint, lookaheadDist);
        lastLookahead = lookaheadPose; //* update last lookahead position FOR DEVIATION FIXES

        dataLine.append("lookahead x: " + std::to_string(lookaheadPose.x) + "\n");
        dataLine.append("lookahead y: " + std::to_string(lookaheadPose.y) + "\n");

        curvature = findLookaheadCurvature(pose, M_PI / 2 - (pose.theta), lookaheadPose);
        // if(curvature<0.001 && curvature > -0.001) {
        //     curvature = 0;
        //     dataLine.append("zero");
        // }
        dataLine.append("curvature: " + std::to_string(curvature) + "\n"); //write curvature

        targetVel = std::stof(velocities.at(closestPoint));

        dataLine.append("target vel: " + std::to_string(targetVel) + "\n"); //write target vel

        if(pathID == "Red WP") {
            switch(std::stoi(subValues.at(closestPoint)[5])) {
                case 1:
                    targetVel *= 1;
                    break;
                case 2:
                    targetVel *= 1;
                    break;
            }
        } else if(pathID == "Red ")

        // calculate target left and right velocities
        targetLeftVel = targetVel * (2 + curvature * drivetrain.trackWidth) / 2;
        targetRightVel = targetVel * (2 - curvature * drivetrain.trackWidth) / 2;

        //fallback exclusion
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
        ratio = std::max(std::fabs(targetLeftVel), std::fabs(targetRightVel)) / 12000;
        if (ratio > 1) {
            targetLeftVel /= ratio;
            targetRightVel /= ratio;
        }

        // write velocities
        dataLine.append("current velocities: " + std::to_string(leftMotors.get_voltage()) + " " + std::to_string(rightMotors.get_voltage()) + "\n");
        dataLine.append("target vels: " + std::to_string(targetLeftVel) + " " + std::to_string(targetRightVel) + "\n\n");

        // send velocity
        leftMotors.move_voltage(targetLeftVel);
        rightMotors.move_voltage(targetRightVel);
        
        fileOThree << dataLine;
        fileOThree.flush();

        pros::delay(10);
    }

    // stop the robot
    drivetrain.leftMotors->move(0);
    drivetrain.rightMotors->move(0);
    // set distTraveled to -1 to indicate that the function has finished
    distTraveled = -1;
    // give the mutex back
    this->endMotion();
}