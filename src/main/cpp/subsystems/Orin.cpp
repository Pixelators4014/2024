// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <frc/smartdashboard/SmartDashboard.h>
#include <networktables/NetworkTableInstance.h>

#include "subsystems/Orin.h"

Orin::Orin() {}

void Orin::Periodic() {
  double x = table->GetNumber("x", 0.0);
  double y = table->GetNumber("y", 0.0);
  double z = table->GetNumber("z", 0.0);
  double theta = table->GetNumber("theta", 0.0);
  double dx = table->GetNumber("dx", 0.0);
  double dy = table->GetNumber("dy", 0.0);
  double dz = table->GetNumber("dz", 0.0);
  double dtheta = table->GetNumber("dtheta", 0.0);

  frc::SmartDashboard::PutNumber("x", x);
  frc::SmartDashboard::PutNumber("y", y);
  frc::SmartDashboard::PutNumber("z", z);
  frc::SmartDashboard::PutNumber("theta", theta);
  frc::SmartDashboard::PutNumber("dx", dx);
  frc::SmartDashboard::PutNumber("dy", dy);
  frc::SmartDashboard::PutNumber("dz", dz);
  frc::SmartDashboard::PutNumber("dtheta", dtheta);
}
