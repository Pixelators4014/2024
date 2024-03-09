// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <numbers>

#include <frc/ADIS16470_IMU.h>
#include <frc/geometry/Translation2d.h>
#include <frc/kinematics/SwerveDriveKinematics.h>
#include <frc/kinematics/SwerveDriveOdometry.h>

#include "SwerveModule.h"
#include "Constants.h"

/**
 * Represents a swerve drive style drivetrain.
 */
class Drivetrain
{
public:
  Drivetrain()
  {
    m_IMU.Reset();
  }

  void Drive(units::meters_per_second_t xSpeed,
             units::meters_per_second_t ySpeed, units::radians_per_second_t rot,
             bool fieldRelative, units::second_t period);
  void UpdateOdometry();

private:
  frc::Translation2d m_frontLeftLocation{kRobotRadius, kRobotRadius};
  frc::Translation2d m_frontRightLocation{kRobotRadius, -kRobotRadius};
  frc::Translation2d m_backLeftLocation{-kRobotRadius, kRobotRadius};
  frc::Translation2d m_backRightLocation{-kRobotRadius, -kRobotRadius};

  SwerveModule m_backLeft{kBackLeftTurningMotorID, kBackLeftDriveMotorID, kBackLeftCancoderID, kBackLeftAngleOffset};
  SwerveModule m_frontLeft{kFrontLeftTurningMotorID, kFrontLeftDriveMotorID, kFrontLeftCancoderID, kFrontLeftAngleOffset};
  SwerveModule m_frontRight{kFrontRightTurningMotorID, kFrontRightDriveMotorID, kFrontRightCancoderID, kFrontRightAngleOffset};
  SwerveModule m_backRight{kBackRightTurningMotorID, kBackRightDriveMotorID, kBackRightCancoderID, kBackRightAngleOffset};

  frc::ADIS16470_IMU m_IMU;

  frc::SwerveDriveKinematics<4> m_kinematics{
      m_frontLeftLocation, m_frontRightLocation, m_backLeftLocation,
      m_backRightLocation};

  frc::SwerveDriveOdometry<4> m_odometry{
      m_kinematics,
      m_IMU.GetAngle(),
      {m_frontLeft.GetPosition(), m_frontRight.GetPosition(),
       m_backLeft.GetPosition(), m_backRight.GetPosition()}};
};
