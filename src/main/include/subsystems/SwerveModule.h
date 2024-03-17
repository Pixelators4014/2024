// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <ctre/phoenix6/CANcoder.hpp>
#include <ctre/phoenix6/TalonFX.hpp>
#include <ctre/phoenix6/controls/DutyCycleOut.hpp>
#include <ctre/phoenix6/controls/PositionVoltage.hpp>
#include <frc/kinematics/SwerveModulePosition.h>
#include <frc/kinematics/SwerveModuleState.h>

class SwerveModule {
public:
  SwerveModule(int turningMotorID, int driveMotorID, int cancoderID,
               units::angle::turn_t angleOffset);
  frc::SwerveModuleState GetState();
  frc::SwerveModulePosition GetPosition();
  void SetDesiredState(const frc::SwerveModuleState &state);

private:
  ctre::phoenix6::hardware::TalonFX m_driveMotor;
  ctre::phoenix6::hardware::TalonFX m_turningMotor;

  ctre::phoenix6::hardware::CANcoder angleEncoder;

  ctre::phoenix6::controls::PositionVoltage anglePosition;
  ctre::phoenix6::controls::DutyCycleOut driveDutyCycle;
};
