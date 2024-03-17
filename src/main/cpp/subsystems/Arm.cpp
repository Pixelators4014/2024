// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystems/Arm.h"
#include "units/angle.h"

Arm::Arm()
    : m_leftMotor(kLeftArmMotorID), m_rightMotor(kRightArmMotorID),
      anglePosition(0_tr) {}

void Arm::SetDesiredPosition(units::angle::turn_t turns) {
  m_leftMotor.SetControl(anglePosition.WithPosition(turns * kArmGearRatio));
  m_rightMotor.SetControl(anglePosition.WithPosition(turns * kArmGearRatio));
}

units::angle::turn_t Arm::GetPosition() {
  return m_leftMotor.GetPosition().GetValue() / kArmGearRatio;
}

// This method will be called once per scheduler run
void Arm::Periodic() {}
