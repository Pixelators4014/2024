// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystems/Grabber.h"

Grabber::Grabber() : m_motor(kGrabberMotorID), driveDutyCycle(0) {
  m_motor.SetInverted(KGrabberMotorInverted);
}

void Grabber::SetSpeed(const units::dimensionless::scalar_t output) {
  m_motor.SetControl(driveDutyCycle.WithOutput(output));
}

bool Grabber::IsLowerBeamBroken() { return lowerBreakBeam.Get(); }

bool Grabber::IsUpperBeamBroken() { return upperBreakBeam.Get(); }

bool Grabber::IsFullyInserted() {
  return !IsLowerBeamBroken() && IsUpperBeamBroken();
}

// This method will be called once per scheduler run
void Grabber::Periodic() {}
