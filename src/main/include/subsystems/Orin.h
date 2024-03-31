// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include <netinet/in.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>

#include <kissnet/kissnet.hpp>
namespace kn = kissnet;

#define SERVER_TARGET_IP "10.40.14.49"
#define SERVER_TARGET_PORT 5800
#define BUFFER_SIZE 32

struct Pose {
  float x;
  float y;
  float z;
  float roll;
  float pitch;
  float yaw;
};

class Orin : public frc2::SubsystemBase {
public:
  Orin();

  /**
   * Will be called periodically whenever the CommandScheduler runs.
   */
  void Periodic() override;

private:
  Pose pose;

  void getPose();

  int setPose(Pose pose);
};
