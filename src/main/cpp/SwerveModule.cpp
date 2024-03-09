// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "SwerveModule.h"
#include "Constants.h"

#include <frc/smartdashboard/SmartDashboard.h>

SwerveModule::SwerveModule(const int turningMotorID, const int driveMotorID, const int cancoderID, const units::angle::turn_t angleOffset)
    : m_driveMotor(driveMotorID),
      m_turningMotor(turningMotorID),
      angleEncoder(cancoderID),
      anglePosition(0_tr),
      driveDutyCycle(0)
{
  ctre::phoenix6::configs::TalonFXConfiguration swerve_drive_FX_config = ctre::phoenix6::configs::TalonFXConfiguration();
  ctre::phoenix6::configs::TalonFXConfiguration swerve_angle_FX_config = ctre::phoenix6::configs::TalonFXConfiguration();
  ctre::phoenix6::configs::CANcoderConfiguration swerve_cancoder_config = ctre::phoenix6::configs::CANcoderConfiguration();

  swerve_cancoder_config.MagnetSensor.SensorDirection = KCancoderSensorDirection;

  swerve_drive_FX_config.MotorOutput.Inverted = KDriveMotorInverted;
  swerve_drive_FX_config.MotorOutput.NeutralMode = KDriveMotorNeutralMode;

  swerve_drive_FX_config.Feedback.SensorToMechanismRatio = KDriveSensorToMechanismRatio;

  swerve_drive_FX_config.CurrentLimits.SupplyCurrentLimitEnable = KDriveCurrentLimitEnable;
  swerve_drive_FX_config.CurrentLimits.SupplyCurrentLimit = KDriveCurrentLimit;
  swerve_drive_FX_config.CurrentLimits.SupplyCurrentThreshold = KDriveCurrentThreshold;
  swerve_drive_FX_config.CurrentLimits.SupplyTimeThreshold = KDriveTimeThreshold;

  swerve_drive_FX_config.Slot0.kP = KDriveSlot0kP;
  swerve_drive_FX_config.Slot0.kI = KDriveSlot0kI;
  swerve_drive_FX_config.Slot0.kD = KDriveSlot0kD;

  swerve_drive_FX_config.OpenLoopRamps.DutyCycleOpenLoopRampPeriod = KDriveDutyCycleOpenLoopRampPeriod;
  swerve_drive_FX_config.OpenLoopRamps.VoltageOpenLoopRampPeriod = KDriveVoltageOpenLoopRampPeriod;

  swerve_drive_FX_config.ClosedLoopRamps.DutyCycleClosedLoopRampPeriod = KDriveDutyCycleClosedLoopRampPeriod;
  swerve_drive_FX_config.ClosedLoopRamps.VoltageClosedLoopRampPeriod = KDriveVoltageClosedLoopRampPeriod;

  swerve_angle_FX_config.MotorOutput.Inverted = KAngleMotorInverted;

  swerve_angle_FX_config.Feedback.SensorToMechanismRatio = KAngleSensorToMechanismRatio;
  swerve_angle_FX_config.ClosedLoopGeneral.ContinuousWrap = KAngleContinuousWrap;

  swerve_angle_FX_config.CurrentLimits.SupplyCurrentLimitEnable = KAngleCurrentLimitEnable;
  swerve_angle_FX_config.CurrentLimits.SupplyCurrentLimit = KAngleCurrentLimit;
  swerve_angle_FX_config.CurrentLimits.SupplyCurrentThreshold = KAngleCurrentThreshold;
  swerve_angle_FX_config.CurrentLimits.SupplyTimeThreshold = KAngleTimeThreshold;

  swerve_angle_FX_config.Slot0.kP = KAngleSlot0kP;
  swerve_angle_FX_config.Slot0.kI = KAngleSlot0kI;
  swerve_angle_FX_config.Slot0.kD = KAngleSlot0kD;

  anglePosition.EnableFOC = EnableFOC;
  driveDutyCycle.EnableFOC = EnableFOC;

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

  m_turningMotor.SetControl(anglePosition.WithPosition(units::turn_t{state.angle.Radians()}));
  m_driveMotor.SetControl(driveDutyCycle.WithOutput(state.speed / kMaxSpeed));
}
