// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "SwerveModule.h"

#include <numbers>

#include <frc/geometry/Rotation2d.h>

SwerveModule::SwerveModule(const int driveMotorID, const int turningMotorID, const int cancoderID, const units::angle::turn_t angleOffset)
    : m_driveMotor(driveMotorID),
      m_turningMotor(turningMotorID),
      angleEncoder(cancoderID),
      anglePosition(0_tr),
      driveDutyCycle(0)
{
  ctre::phoenix6::configs::TalonFXConfiguration swerve_drive_FX_config = ctre::phoenix6::configs::TalonFXConfiguration();
  ctre::phoenix6::configs::TalonFXConfiguration swerve_angle_FX_config = ctre::phoenix6::configs::TalonFXConfiguration();
  ctre::phoenix6::configs::CANcoderConfiguration swerve_cancoder_config = ctre::phoenix6::configs::CANcoderConfiguration();

  swerve_cancoder_config.MagnetSensor.SensorDirection = ctre::phoenix6::signals::SensorDirectionValue::Clockwise_Positive;

  swerve_drive_FX_config.MotorOutput.Inverted = ctre::phoenix6::signals::InvertedValue::Clockwise_Positive;
  swerve_drive_FX_config.MotorOutput.NeutralMode = ctre::phoenix6::signals::NeutralModeValue::Brake;

  swerve_drive_FX_config.Feedback.SensorToMechanismRatio = 6.12;

  swerve_drive_FX_config.CurrentLimits.SupplyCurrentLimitEnable = true;
  swerve_drive_FX_config.CurrentLimits.SupplyCurrentLimit = 40;
  swerve_drive_FX_config.CurrentLimits.SupplyCurrentThreshold = 40;
  swerve_drive_FX_config.CurrentLimits.SupplyTimeThreshold = 0.1;

  swerve_drive_FX_config.Slot0.kP = 1.0;
  swerve_drive_FX_config.Slot0.kI = 0.0;
  swerve_drive_FX_config.Slot0.kD = 0.0;

  swerve_drive_FX_config.OpenLoopRamps.DutyCycleOpenLoopRampPeriod = 0.1;
  swerve_drive_FX_config.OpenLoopRamps.VoltageOpenLoopRampPeriod = 0.1;

  swerve_drive_FX_config.ClosedLoopRamps.DutyCycleClosedLoopRampPeriod = 0.1;
  swerve_drive_FX_config.ClosedLoopRamps.VoltageClosedLoopRampPeriod = 0.1;

  swerve_angle_FX_config.MotorOutput.Inverted = ctre::phoenix6::signals::InvertedValue::Clockwise_Positive;

  swerve_angle_FX_config.Feedback.SensorToMechanismRatio = 150.0 / 7.0;
  swerve_angle_FX_config.ClosedLoopGeneral.ContinuousWrap = true;

  swerve_angle_FX_config.CurrentLimits.SupplyCurrentLimitEnable = true;
  swerve_angle_FX_config.CurrentLimits.SupplyCurrentLimit = 40;
  swerve_angle_FX_config.CurrentLimits.SupplyCurrentThreshold = 40;
  swerve_angle_FX_config.CurrentLimits.SupplyTimeThreshold = 0.1;

  swerve_angle_FX_config.Slot0.kP = 1.0;
  swerve_angle_FX_config.Slot0.kI = 0.0;
  swerve_angle_FX_config.Slot0.kD = 0.0;

  m_driveMotor.GetConfigurator().Apply(swerve_drive_FX_config);
  m_turningMotor.GetConfigurator().Apply(swerve_angle_FX_config);
  angleEncoder.GetConfigurator().Apply(swerve_cancoder_config);

  m_turningMotor.SetPosition(angleEncoder.GetAbsolutePosition().GetValue() - angleOffset);

  m_driveMotor.GetConfigurator().SetPosition(0_rad);
}

frc::SwerveModuleState SwerveModule::GetState()
{ 
  return {m_driveMotor.GetVelocity().GetValue() * kTurnsToMeters,
        m_turningMotor.GetPosition().GetValue()};
}

frc::SwerveModulePosition SwerveModule::GetPosition()
{
  return frc::SwerveModulePosition(m_driveMotor.GetPosition().GetValue() * kTurnsToMeters,
          m_turningMotor.GetPosition().GetValue());
}

void SwerveModule::SetDesiredState(
    const frc::SwerveModuleState &referenceState)
{

  const units::angle::turn_t angle = m_turningMotor.GetPosition().GetValue();

  frc::SwerveModuleState state = frc::SwerveModuleState::Optimize(referenceState, angle);

  m_turningMotor.SetControl(anglePosition.WithPosition(state.angle.Degrees() * 360));
  m_driveMotor.SetControl(driveDutyCycle.WithOutput(state.speed / 3.0_mps)); // TODO
}
