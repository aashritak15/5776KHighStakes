#include "main.h"
#include "subsystems/ports.hpp"
#include "subsystems/intake.hpp"
#include "globals.hpp"

using namespace okapi;

void intakeInnit() { intakeMotor.setBrakeMode(AbstractMotor::brakeMode::coast); }

Motor intakeMotor(intakePort1, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

ControllerButton intakeButton = ControllerButton(ControllerDigital::L1);

void updateIntake() {
    static IntakeState currentIntakeState = IntakeState::STOPPED;
    // static IntakeState previousIntakeState = IntakeState::STOPPED;

    if (intakeButton.changedToPressed()) {
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
    if (outakeButton.changedToPressed()) {
        if (currentIntakeState == IntakeState::OUTTAKING) {
            // previousIntakeState = currentIntakeState;
            currentIntakeState = IntakeState::STOPPED;
        } else {
            // previousIntakeState = currentIntakeState;
            currentIntakeState = IntakeState::OUTTAKING;
        }
    }

    switch (currentIntakeState) {
        case IntakeState::STOPPED:
            // gradualStop();
            intakeMotor.moveVoltage(0);

            break;
        case IntakeState::INTAKING: intakeMotor.moveVoltage(12000); break;
        case IntakeState::OUTTAKING:
            intakeMotor.moveVoltage(-12000);
            break;
            // case IntakeState::HALF:
    }
}

IntakeState getIntakeState() { return currentIntakeState; }

void setIntakeState(IntakeState IState) { currentIntakeState = IState; }