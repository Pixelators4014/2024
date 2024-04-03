// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <ctre/phoenix6/TalonFX.hpp>
#include <ctre/phoenix6/controls/DutyCycleOut.hpp>
#include <frc/DigitalInput.h>
#include <frc2/command/SubsystemBase.h>

#include "Constants.h"

class Grabber : public frc2::SubsystemBase {
public:
  Grabber();

  void SetSpeed(const units::dimensionless::scalar_t output);

  bool IsLowerBeamBroken();
  bool IsUpperBeamBroken();
  bool IsFullyInserted();

  /**
   * Will be called periodically whenever the CommandScheduler runs.
   */
  void Periodic() override;

private:
  // Components (e.g. motor controllers and sensors) should generally be
  // declared private and exposed only through public methods.

  ctre::phoenix6::hardware::TalonFX m_motor;
  frc::DigitalInput lowerBreakBeam{kLowerBreakBeamID};
  frc::DigitalInput upperBreakBeam{kUpperBreakBeamID};

  ctre::phoenix6::controls::DutyCycleOut driveDutyCycle;
};
