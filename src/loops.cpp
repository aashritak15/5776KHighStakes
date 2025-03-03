#include "loops.hpp"
#include "magic.hpp"

void matchControl() {
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    controller.set_text(0, 0, "no sort");
    while (true) {
        
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        chassis.arcade(leftY, rightX * 0.85); // 0.9

        updateIntake();
        updateColorSort();
        updateClamp();
        updateDoinkRight();
        updateDoinkLeft();
        //updateIntakePiston();
        // updateLB();

        // std::cout<<std::to_string(optical.get_proximity())<<"\n";

        pros::delay(10);
    }
}

void rerunControl() {    
    initO();

    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

    int count = 1;

    while (true) {
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        chassis.arcade(leftY, rightX * 0.85);

        updateIntake();
        //updateColorSort();
        updateClamp();
        updateDoinkRight();
        updateDoinkLeft();
        updateIntakePiston();
        //updateLB();

        if (count == 5) { //*data written every 0.05 seconds
            writePose();
            writeAdditional();
            count = 1;
            fileO.flush();
            fileOTwo.flush();
        }

        count++;

        closeO();

        pros::delay(10);
    }
}