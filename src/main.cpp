#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "ports.hpp"
#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "lift.hpp"
#include "intakeFirst.hpp"
#include "intakePiston.hpp"

pros::MotorGroup leftMotors({-3, -1, -16}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({19, 20, 18}, pros::MotorGearset::blue);

pros::Imu imu(2);


// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, 10, lemlib::Omniwheel::NEW_325, 425,
                              6 // 2 w/o traction
);

// lateral motion controller
lemlib::ControllerSettings linearController(13.5, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            30.43,//22, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            0 // slew
);

// angular motion controller
lemlib::ControllerSettings angularController(4.6, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             37.38, // derivative gain (kD)
                                             0, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             2, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // slew
);

// sensors for odometry
lemlib::OdomSensors sensors(nullptr, nullptr, nullptr, nullptr,
                            &imu // inertial sensor
);

// tanish driver functions:

lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors
    clampInit();
    intakeInnit();
    liftInit();
    intakeClampInit();

    // the default rate is 50. however, if you need to change the rate, you
    // can do the following.
    // lemlib::bufferedStdout().setRate(...);
    // If you use bluetooth or a wired connection, you will want to have a rate of 10ms

    // for more information on how the formatting for the loggers
    // works, refer to the fmtlib docs

    // thread to for brain screen and position logging
    pros::Task screenTask([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            pros::lcd::print(3, "Lift: %f", lift.get_position()); //lift encoder
            // log position telemetry
            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
            // delay to save resources
            pros::delay(50);
        }
    });
}

/**
 * Runs while the robot is disabled
 */
void disabled() {}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}

/**
 * Runs during auto
 *
 * This is an example autonomous routine which demonstrates a lot of the features LemLib has to offer
 */
void autonomous() {


   
    double liftPosition = lift.get_position();

    //theta = 32.26
    //go to mogo and clamp
    mogoClamp.set_value(true);
    chassis.moveToPoint(0, -16, 2000, {.forwards = false});
    chassis.moveToPoint(0, -21, 3000, {.forwards = false, .maxSpeed = 50});
    chassis.waitUntilDone();
    
    mogoClamp.set_value(false); 
    intake.move_voltage(-12000);
    pros::delay(150);
    
    
    //go to first ring
    //chassis.turnToHeading(78.2, 800);
    //chassis.moveToPose(11.04, -18.63, 82.26, 2000);
    chassis.moveToPose(11.29, -17.44, 76.95, 2000); 
    intakeFirst.move_velocity(-600);
    //pros::delay(250;0); //could be less IF  ERROR UNCOMMENT THIS
    ;// LINE <---------------------
   
    

    chassis.moveToPose(20.64, -26.97, 125.9, 2000);
     
    chassis.waitUntilDone(); //could be less 
    
    

    //go to point and release mogo
    intake.move_voltage(0);
    chassis.moveToPoint(-5.83, -3.54, 2000, {.forwards = false});
    chassis.waitUntilDone();
    chassis.turnToHeading(249,800);
    
    chassis.waitUntilDone();
    mogoClamp.set_value(true);
    
    chassis.waitUntilDone();
    pros::delay(500);

    //move to alliance
    intakeFirst.move_velocity(0);
    chassis.moveToPose(-27.5, -13.5, 243.33, 3000, {.earlyExitRange = 5});
    chassis.turnToHeading(153, 1000);
    mogoClamp.set_value(false);
    chassis.moveToPoint(-29, -3, 2000,{.forwards = false});
    intake.move_voltage(-12000);
    lift.move_absolute(143, 40);
    pros::delay(1500);
    intake.move_voltage(0);
    lift.move_absolute(liftPosition, -60);

    intakeFirst.move_voltage(12000);
    chassis.moveToPose(-19.98, -31.39, 164, 3000, {.minSpeed = 75});
    

    

    //move to alliance
    /*
    chassis.moveToPoint(5.32, -9.88, 2000, {.forwards = false});
    chassis.moveToPose(-32.6, -2.23, 153.06, 2000, {.forwards = false}); //doesnt push one ring away enough
    lift.move_absolute(265, 100);//lift encoder: 265
    mogoClamp.set_value(false);
*/
    
    /*chassis.moveToPose(-32.1, 0.51, 113.93, 2000);    /*
    intake.move_voltage(0);
    chassis.moveToPose(-33, -16, 59.89, 2000, {.forwards = false});
    chassis.turnToHeading(154, 2000);
    */

    //chassis.moveToPose()
    
    //81.80
    //19, -18.397
} 




/**
 * Runs in driver control
 */


void opcontrol() {

    
    // controller
    // loop to continuously update motors
    while (true) {
            // pros::lcd::set_text(0, "X: %f", chassis.getPose().x); // x
            // pros::lcd::set_text(1, "Y: %f", chassis.getPose().y); // y
            // pros::lcd::set_text(2, "Theta: %f", chassis.getPose().theta); // heading
            // // log position telemetry
            // lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
        // get joystick positions
        pros::lcd::set_text(0,  std::to_string(chassis.getPose().x)); 
        pros::lcd::set_text(1,  std::to_string(chassis.getPose().y));
         pros::lcd::set_text(2, std::to_string(chassis.getPose().theta));
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the chassis with curvature drive
        chassis.arcade(leftY, rightX);
        //chassis.arcade(leftY, rightX, false, 1);
        updateIntake();
        updateIntakeFirst();
        updateClamp();
        updateIntakeClamp();
        updateLift();
        resetIntake();
        stepIntake();
        //allianceStake();
        pros::delay(10);
    }
}
