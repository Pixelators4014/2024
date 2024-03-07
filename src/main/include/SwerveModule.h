// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <numbers>

#include <ctre/phoenix6/CANcoder.hpp>
#include <ctre/phoenix6/TalonFX.hpp>
#include <ctre/phoenix6/controls/PositionVoltage.hpp>
#include <ctre/phoenix6/controls/DutyCycleOut.hpp>
#include <ctre/phoenix6/controls/VelocityVoltage.hpp>
#include <ctre/phoenix6/controls/PositionVoltage.hpp>
#include <frc/Encoder.h>
#include <frc/controller/PIDController.h>
#include <frc/controller/ProfiledPIDController.h>
#include <frc/controller/SimpleMotorFeedforward.h>
#include <frc/kinematics/SwerveModulePosition.h>
#include <frc/kinematics/SwerveModuleState.h>
#include <frc/motorcontrol/PWMSparkMax.h>
#include <units/angular_velocity.h>
#include <units/time.h>
#include <units/velocity.h>
#include <units/voltage.h>

class SwerveModule
{
public:
    SwerveModule(int driveMotorID, int turningMotorID, int cancoderID, units::angle::turn_t angleOffset);
    frc::SwerveModuleState GetState();
    frc::SwerveModulePosition GetPosition();
    void SetDesiredState(const frc::SwerveModuleState &state);

private:
    static constexpr units::meter_t kWheelRadius = 0.0508_m;
    static constexpr auto kTurnsToMeters = 2 * std::numbers::pi * kWheelRadius / 1_tr;

    static constexpr auto kModuleMaxAngularVelocity =
        std::numbers::pi * 1_rad_per_s; // radians per second
    static constexpr auto kModuleMaxAngularAcceleration =
        std::numbers::pi * 2_rad_per_s / 1_s; // radians per second^2

    ctre::phoenix6::hardware::CANcoder angleEncoder;

    ctre::phoenix6::hardware::TalonFX m_driveMotor;
    ctre::phoenix6::hardware::TalonFX m_turningMotor;

    ctre::phoenix6::controls::PositionVoltage anglePosition;
    ctre::phoenix6::controls::DutyCycleOut driveDutyCycle;

};
