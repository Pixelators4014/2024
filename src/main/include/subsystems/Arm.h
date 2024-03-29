// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <ctre/phoenix6/TalonFX.hpp>
#include <frc2/command/SubsystemBase.h>

#include "Constants.h"
#include "units/angle.h"

class Arm : public frc2::SubsystemBase {
public:
    Arm();

    units::angle::turn_t GetPosition();

    void SetDesiredPosition(const units::angle::turn_t turns);

    /**
     * Will be called periodically whenever the CommandScheduler runs.
     */
    void Periodic() override;

private:
    // Components (e.g. motor controllers and sensors) should generally be
    // declared private and exposed only through public methods.
    ctre::phoenix6::hardware::TalonFX m_leftMotor;
    ctre::phoenix6::hardware::TalonFX m_rightMotor;

    ctre::phoenix6::controls::PositionVoltage leftAnglePosition;
    ctre::phoenix6::controls::PositionVoltage rightAnglePosition;
};
