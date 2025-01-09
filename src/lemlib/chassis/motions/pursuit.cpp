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
        // lemlib::infoSink()->debug("read raw line {}", stringToHex(line));
        
        if (line == "endData" || line == "endData\r") break;

        const std::vector<std::string> pointInput = readElement(line, ", "); // parse line

        robotVelocities.push_back(pointInput.at(3));
        // lemlib::infoSink()->debug("read point {}", pathPoint);
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

        // if (pointInput[0].size() != 2) { possibly add back later when we aren't playing w this function
        //     std::cout<<"sub size: "<<pointInput.size()<<"\n";
        //     lemlib::infoSink()->error("Failed to read sub file! Are you using the right format? Raw line: {}",
        //                               stringToHex(line));
        //     break;
        // }
        
        // std::cout<<"reverse: "<<temp.at(0)<<"\n"; //* This works!!
        // std::cout<<"intake: "<<temp.at(1)<<"\n";
        // std::cout<<temp.at(2)<<"\n\n";

    }

    // for(int j=0; j<pointInput.size(); j++){
    //     for(int k=0; k<pointInput[j].size(); k++)
    //         std::cout<<pointInput[j][k] + ", ";
    //     std::cout<<"\n";
    // }

    return pointInput;
}

/**
 * @brief find the closest point on the path to the robot
 *
 * @param pose the current pose of the robot
 * @param path the path to follow
 * @return int index to the closest point
 */
int findClosest(lemlib::Pose pose, std::vector<lemlib::Pose> path, int skips, int prevIndex) {
    int closestPoint;
    int skipsLeft = skips;
    float closestDist = infinity();
    int maxIndex;
    if (prevIndex + 5 > path.size()) {
        maxIndex = path.size();
    } else {
        maxIndex = prevIndex + 5;
    }

    // loop through all path points
    for (int i = prevIndex; i < maxIndex; i++) { 
        const float dist = pose.distance(path.at(i));
        if (dist < closestDist) { // new closest point
            if (skipsLeft == 0) {
                closestDist = dist;
                closestPoint = i;
            } else {
                skipsLeft--;
                std::cout<<"skips left: "<<std::to_string(skipsLeft)<<"\n";
            }
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
    float discriminant = b * b - 4 * a * c;

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
lemlib::Pose lookaheadPoint(lemlib::Pose lastLookahead, lemlib::Pose pose, std::vector<lemlib::Pose> path, int closest,
                            float lookaheadDist) {
    // optimizations applied:
    // only consider intersections that have an index greater than or equal to the point closest
    // to the robot
    // and intersections that have an index greater than or equal to the index of the last
    // lookahead point
    const int start = std::max(closest, int(lastLookahead.theta));
    for (int i = start; i < path.size() - 1; i++) {
        lemlib::Pose lastPathPose = path.at(i);
        lemlib::Pose currentPathPose = path.at(i + 1);

        float t = circleIntersect(lastPathPose, currentPathPose, pose, lookaheadDist);

        if (t != -1) {
            lemlib::Pose lookahead = lastPathPose.lerp(currentPathPose, t);
            lookahead.theta = i;
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

    // return curvature
    return side * ((2 * x) / (d * d));
}

void lemlib::Chassis::follow(const asset& path, const asset& sub, float lookahead, int timeout, bool forwards, bool async) { //* forwards and async unused
    
    std::cout<<"rerun started\n";

    this->requestMotionStart();

    std::vector<lemlib::Pose> pathPoints = getData(path); // get list of path points
    std::vector<std::vector<std::string>> subValues = getSubData(sub); //get subsystem values
    std::vector<std::string> velocities = getVelocities(path); //get velocities

    Pose pose = this->getPose(true);
    Pose lastPose = pose;
    Pose lookaheadPose(0, 0, 0); 
    Pose lastLookahead = pathPoints.at(0);
    lastLookahead.theta = 0;
    float curvature;
    float targetVel;
    float prevLeftVel = 0;
    float prevRightVel = 0;
    int closestPoint = 0; //*does this work
    float leftInput = 0;
    float rightInput = 0;
    float prevVel = 0;
    int compState = pros::competition::get_status();
    int skips = 0;
    distTraveled = 0;

    // loop until the robot is within the end tolerance
    for (int i = 0; i < timeout / 10 && pros::competition::get_status() == compState && this->motionRunning; i++) { //* compState and motionRunning??? remove
        // if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)){ 
        // // logic: copy file until stop point (copy number of lines from closestPoint) for both files
        // //       copy coords and heading to separate file (i dont wanna break stuff by switching to driver in the middle of auton)
        // //       in main: start writing new file but initialize bot position with coords and heading
        // //                combine files when done
        //     std::ifstream file0("/usd/autonomous.txt", std::ios::app);
        //     std::ifstream file0Two("/usd/extra.txt", std::ios::app);
        //     std::ofstream newFile0("static/newAuton");
        //     std::ofstream newFile0Two("static/newExtra");
        //     std::string temp;
        //     for(int kiwi=0; kiwi<closestPoint; kiwi++){
        //         std::getline(std::cin, temp);
        //     }
        //     break;
        // }
        
        std::string dataLine = "";

        closestPoint = findClosest(pose, pathPoints, skips, closestPoint); //TODO: optimize quite vile

        dataLine.append("NEW TICK\ntarget index: " + std::to_string(closestPoint) + "\n");
        dataLine.append("skips: " + std::to_string(skips) + "\n");
        dataLine.append("x: " + std::to_string(pose.x) + "\n");
        dataLine.append("y: " + std::to_string(pose.y) + "\n");
        dataLine.append("theta: " + std::to_string(pose.theta) + "\n");

        // get the current position of the robot
        pose = this->getPose(true);

        if (subValues.at(closestPoint)[2] == "STOPPED") { //*primary exclusion for delays
            drivetrain.leftMotors->move(0);                 
            drivetrain.rightMotors->move(0);
            dataLine.append("DELAYED\n\n");
            fileOThree<<dataLine;
            fileOThree.flush();
            // std::cout<<"Delayed\n\n";
            pros::delay(100); //*change to tick speed always
            closestPoint++;
            continue;

        } else if(subValues.at(closestPoint)[2] == "TURNING CW" || subValues.at(closestPoint)[2] == "TURNING CCW" ) { //*exclusion for turns (pids are back)
            this->endMotion();

            pros::delay(10); //TODO: see if this fixes it
            
            int prevClosestPoint = closestPoint;
            //chassis.setPose(pose.x, pose.y, 0);

            closestPoint++; //TODO: single cw exclusion
            while(subValues.at(closestPoint)[2] == "TURNING CW" || subValues.at(closestPoint)[2] == "TURNING CCW") {closestPoint++;}
            closestPoint--; 

            dataLine.append("turn end index: " + std::to_string(closestPoint) + "\n");
            std::cout<<"turn end index: "<<std::to_string(closestPoint)<<"\n";

            float turnDist = pathPoints.at(closestPoint).theta;//-pose.theta;
            
            dataLine.append("turn distance: " + std::to_string(turnDist) + "\n");
            std::cout<<"turn distance: "<<std::to_string(turnDist)<<"\n";

            if (subValues.at(prevClosestPoint)[2] == "TURNING CW") {
                dataLine.append("begin theta cw: " + std::to_string(pose.theta) + "\n");
                std::cout<<"BEGIN THETA CW";
                chassis.turnToHeading(turnDist, 10000, {.direction = AngularDirection::CW_CLOCKWISE}, true);
                std::cout<<"END THETA CW";
            } else {
                dataLine.append("begin theta ccw: " + std::to_string(pose.theta) + "\n");
                std::cout<<"BEGIN THETA CCW";
                chassis.turnToHeading(turnDist, 10000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE}, true);
                std::cout<<"END THETA CCW";
                dataLine.append("end theta ccw: " + std::to_string(pose.theta) + "\n\n");
            }

            //chassis.setPose(pose.x, pose.y, pathPoints.at(closestPoint).theta);

            closestPoint++;

            fileOThree<<dataLine;
            
            fileOThree.flush();

            this->requestMotionStart();

            continue;

        } else if(subValues.at(closestPoint)[2] == "SUBSYS") {
            
            if (subValues.at(closestPoint)[1] == "0") {
            intake.move_voltage(0);
            } else if (subValues.at(closestPoint)[1] == "1") {
                intake.move_voltage(-12000);
            } else if (subValues.at(closestPoint)[1] == "2") {
                intake.move_voltage(12000);
            }

            if (subValues.at(closestPoint)[3] == "0") {
                mogoClamp.set_value(true);
            } else if (subValues.at(closestPoint)[3] == "2") {
                mogoClamp.set_value(false);
            }

            closestPoint++;

            dataLine.append("SUBSYSTEM\n\n");
            fileOThree<<dataLine;
            fileOThree.flush();

            pros::delay(100);

            continue;
        }

        // update completion vars
        distTraveled += pose.distance(lastPose);
        lastPose = pose;

        // find the closest point on the path to the robot

        // std::cout<<"target index: "<<std::to_string(closestPoint)<<"\n";

        if (subValues.at(closestPoint)[1] == "0") {
            intake.move_voltage(0);
        } else if (subValues.at(closestPoint)[1] == "1") {
            intake.move_voltage(-12000);
        } else if (subValues.at(closestPoint)[1] == "2") {
            intake.move_voltage(12000);
        }


        if (subValues.at(closestPoint)[3] == "0") {
            mogoClamp.set_value(true);
        } else if (subValues.at(closestPoint)[3] == "2") {
            mogoClamp.set_value(false);
        }        

        dataLine.append("target x: " + std::to_string(pathPoints.at(closestPoint).x) + "\n");
        dataLine.append("target y: " + std::to_string(pathPoints.at(closestPoint).y) + "\n");

        // std::cout<<"target x: "<<std::to_string(pathPoints.at(closestPoint).x)<<"\n";
        // std::cout<<"target y: "<<std::to_string(pathPoints.at(closestPoint).y)<<"\n";

        // find the lookahead point
        lookaheadPose = lookaheadPoint(lastLookahead, pose, pathPoints, closestPoint, lookahead);
        lastLookahead = lookaheadPose; // update last lookahead position

        // get the curvature of the arc between the robot and the lookahead point
        dataLine.append("target vel: " + std::to_string(targetVel) + "\n");
        // std::cout<<"target vel: "<<std::to_string(targetVel)<<"\n";
        float curvatureHeading = M_PI / 2 - pose.theta; 
        curvature = findLookaheadCurvature(pose, curvatureHeading, lookaheadPose);

        dataLine.append("curvature: " + std::to_string(curvature) + "\n");
        // std::cout<<"curvature: "<<std::to_string(curvature)<<"\n";

        // get the target velocity of the robot
        targetVel = std::stof(velocities.at(closestPoint));

        // targetVel = slew(targetVel, prevVel, lateralSettings.slew); //*i got rid of slew lol
        prevVel = targetVel;

        // calculate target left and right velocities
        float targetLeftVel = targetVel * (2 + curvature * drivetrain.trackWidth) / 2; 
        float targetRightVel = targetVel * (2 - curvature * drivetrain.trackWidth) / 2;

        if ((std::abs(targetLeftVel) < 3.0) && (std::abs(targetRightVel) < 3.0)) { //*secondary exclusion for if bot feels cute
            // std::cout<<"tiny vel\n\n";
            dataLine.append("small vel\n\n");
            pros::delay(10); //*change to tick speed always
            closestPoint++;
            continue;
        }

        // ratio the speeds to respect the max speed
        float ratio = std::max(std::fabs(targetLeftVel), std::fabs(targetRightVel)) / 127;
        if (ratio > 1) {
            targetLeftVel /= ratio;
            targetRightVel /= ratio;
        }

        // update previous velocities //* not important right
        // prevLeftVel = targetLeftVel;
        // prevRightVel = targetRightVel;

        dataLine.append("target vels: " + std::to_string(targetLeftVel) + " " + std::to_string(targetRightVel) + "\n\n");
        // std::cout<<"velocities: "<<targetLeftVel<<", "<<targetRightVel<<"\n\n"; //\n

        // move the drivetrain
        if (subValues.at(closestPoint)[0] == "0") {
            drivetrain.leftMotors->move(targetLeftVel);
            drivetrain.rightMotors->move(targetRightVel);
            std::cout<<"forward ";
        } else if(subValues.at(closestPoint)[0] == "1") {
            drivetrain.leftMotors->move(-targetRightVel);
            drivetrain.rightMotors->move(-targetLeftVel);
            std::cout<<"backward ";
        }

        if (closestPoint == subValues.size() - 2) {
            drivetrain.leftMotors->move(0);
            drivetrain.rightMotors->move(0);
            // set distTraveled to -1 to indicate that the function has finished
            distTraveled = -1;
                std::cout<<"stopped\n";

            // give the mutex back
            this->endMotion();

            fileOThree << dataLine;
            fileOThree.flush();

            return;
        }
        
        fileOThree << dataLine;
        fileOThree.flush();

        std::cout<<"closest point: "<<closestPoint<<"\n";

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