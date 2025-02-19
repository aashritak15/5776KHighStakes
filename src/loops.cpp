#include "loops.hpp"

void matchControl() {
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    while (true) {
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        chassis.arcade(leftY, rightX); // 0.9

        updateIntake();
        updateColorSort();
        updateClamp();
        updateDoink();
        updateLB();
        // lbTask();

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

        chassis.arcade(leftY, rightX);

        updateIntake();
        updateColorSort();
        updateClamp();
        updateDoink();
        updateLB();

        if (count == 5) { //*data written every 0.1 seconds
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