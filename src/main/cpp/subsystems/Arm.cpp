// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystems/Arm.h"
#include "units/angle.h"

Arm::Arm()
    : m_leftMotor(kLeftArmMotorID), m_rightMotor(kRightArmMotorID),
      leftAnglePosition(0_tr), rightAnglePosition(0_tr) {

  ctre::phoenix6::configs::TalonFXConfiguration left_motor_FX_config =
      ctre::phoenix6::configs::TalonFXConfiguration();
  ctre::phoenix6::configs::TalonFXConfiguration right_motor_FX_config =
      ctre::phoenix6::configs::TalonFXConfiguration();

  left_motor_FX_config.Feedback.SensorToMechanismRatio = kArmGearRatio;
  right_motor_FX_config.Feedback.SensorToMechanismRatio = kArmGearRatio;

  left_motor_FX_config.MotorOutput.Inverted = kLeftArmMotorInverted;
  right_motor_FX_config.MotorOutput.Inverted = kRightArmMotorInverted;

  left_motor_FX_config.MotorOutput.NeutralMode = kArmNeutralMode;
  right_motor_FX_config.MotorOutput.NeutralMode = kArmNeutralMode;

  left_motor_FX_config.CurrentLimits.SupplyCurrentLimitEnable =
      kArmCurrentLimitEnable;
  right_motor_FX_config.CurrentLimits.SupplyCurrentLimitEnable =
      kArmCurrentLimitEnable;

  left_motor_FX_config.CurrentLimits.SupplyCurrentLimit = kArmCurrentLimit;
  right_motor_FX_config.CurrentLimits.SupplyCurrentLimit = kArmCurrentLimit;

  left_motor_FX_config.CurrentLimits.SupplyCurrentThreshold =
      kArmCurrentThreshold;
  right_motor_FX_config.CurrentLimits.SupplyCurrentThreshold =
      kArmCurrentThreshold;

  left_motor_FX_config.CurrentLimits.SupplyTimeThreshold = kArmTimeThreshold;
  right_motor_FX_config.CurrentLimits.SupplyTimeThreshold = kArmTimeThreshold;

  left_motor_FX_config.Slot0.kP = kArmSlot0kP;
  right_motor_FX_config.Slot0.kP = kArmSlot0kP;

  left_motor_FX_config.Slot0.kI = kArmSlot0kI;
  right_motor_FX_config.Slot0.kI = kArmSlot0kI;

  left_motor_FX_config.Slot0.kD = kArmSlot0kD;
  right_motor_FX_config.Slot0.kD = kArmSlot0kD;

  left_motor_FX_config.OpenLoopRamps.DutyCycleOpenLoopRampPeriod =
      kArmDutyCycleOpenLoopRampPeriod;
  right_motor_FX_config.OpenLoopRamps.DutyCycleOpenLoopRampPeriod =
      kArmDutyCycleOpenLoopRampPeriod;

  left_motor_FX_config.OpenLoopRamps.VoltageOpenLoopRampPeriod =
      kArmVoltageOpenLoopRampPeriod;
  right_motor_FX_config.OpenLoopRamps.VoltageOpenLoopRampPeriod =
      kArmVoltageOpenLoopRampPeriod;

  left_motor_FX_config.ClosedLoopRamps.DutyCycleClosedLoopRampPeriod =
      kArmDutyCycleClosedLoopRampPeriod;
  right_motor_FX_config.ClosedLoopRamps.DutyCycleClosedLoopRampPeriod =
      kArmDutyCycleClosedLoopRampPeriod;

  left_motor_FX_config.ClosedLoopRamps.VoltageClosedLoopRampPeriod =
      kArmVoltageClosedLoopRampPeriod;
  right_motor_FX_config.ClosedLoopRamps.VoltageClosedLoopRampPeriod =
      kArmVoltageClosedLoopRampPeriod;

  m_leftMotor.GetConfigurator().Apply(left_motor_FX_config);
  m_rightMotor.GetConfigurator().Apply(right_motor_FX_config);

  m_leftMotor.SetPosition(0_tr);
  m_rightMotor.SetPosition(0_tr);
}

void Arm::SetDesiredPosition(units::angle::turn_t turns) {
  m_leftMotor.SetControl(leftAnglePosition.WithPosition(turns));
  m_rightMotor.SetControl(rightAnglePosition.WithPosition(turns));
}

void Arm::Retract() { Arm::SetDesiredPosition(kArmDownTurns); }

void Arm::Extend() { Arm::SetDesiredPosition(kArmUpTurns); }

units::angle::turn_t Arm::GetPosition() {
  return m_leftMotor.GetPosition().GetValue();
}

// This method will be called once per scheduler run
void Arm::Periodic() {}
