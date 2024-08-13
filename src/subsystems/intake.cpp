#include "main.h"
#include "pros/motors.h"
#include "subsystems/ports.hpp"
#include "subsystems/intake.hpp"
#include "globals.hpp"

pros::Motor intake(20, pros::MotorGearset::blue);

void intakeInnit() { intake.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); }

void updateIntake() {
    static IntakeState currentIntakeState = IntakeState::STOPPED;
    // static IntakeState previousIntakeState = IntakeState::STOPPED;

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        if (currentIntakeState == IntakeState::INTAKING) {
            // previousIntakeState = currentIntakeState;
            currentIntakeState = IntakeState::STOPPED;
        } else {
            // previousIntakeState = currentIntakeState;
            currentIntakeState = IntakeState::INTAKING;
        }
    }
    /*
    if (halfInButton.changedToPressed()) {
      if (currentIntakeState == IntakeState::HALF) {
        previousIntakeState = currentIntakeState;
        currentIntakeState = IntakeState::STOPPED;
      } else {
        previousIntakeState = currentIntakeState;
        currentIntakeState = IntakeState::HALF;
      }
    }*/
    // if (outakeButton.changedToPressed()) {
    //     if (currentIntakeState == IntakeState::OUTTAKING) {
    //         // previousIntakeState = currentIntakeState;
    //         currentIntakeState = IntakeState::STOPPED;
    //     } else {
    //         // previousIntakeState = currentIntakeState;
    //         currentIntakeState = IntakeState::OUTTAKING;
    //     }
    // }

    switch (currentIntakeState) {
        case IntakeState::STOPPED: intake.move_velocity(0); break;
        case IntakeState::INTAKING:
            intake.move_velocity(600);
            break;
            // case IntakeState::OUTTAKING: break;
    }
}

IntakeState getIntakeState() { return currentIntakeState; }

void setIntakeState(IntakeState IState) { currentIntakeState = IState; }