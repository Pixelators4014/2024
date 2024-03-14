// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <numbers>

#include <frc/ADIS16470_IMU.h>
#include <frc/geometry/Translation2d.h>
#include <frc/kinematics/SwerveDriveKinematics.h>
#include <frc/kinematics/SwerveDriveOdometry.h>
#include <frc/TimedRobot.h>
#include <frc/DriverStation.h>
#include <frc2/command/SubsystemBase.h>

#include <pathplanner/lib/auto/AutoBuilder.h>
#include <pathplanner/lib/util/HolonomicPathFollowerConfig.h>
#include <pathplanner/lib/util/PIDConstants.h>
#include <pathplanner/lib/util/ReplanningConfig.h>

#include "SwerveModule.h"
#include "Constants.h"

using namespace pathplanner;

/**
 * Represents a swerve drive style drivetrain.
 */
class Drivetrain : public frc2::SubsystemBase
{
public:
  Drivetrain()
  {
    m_IMU.Reset();
    AutoBuilder::configureHolonomic(
        [this](){ return m_odometry.GetPose(); }, // Robot pose supplier
        [this](frc::Pose2d pose){ m_odometry.ResetPosition(m_IMU.GetAngle(), {m_frontLeft.GetPosition(), m_frontRight.GetPosition(), m_backLeft.GetPosition(), m_backRight.GetPosition()}, pose); }, // Method to reset odometry (will be called if your auto has a starting pose)
        [this](){ return m_kinematics.ToChassisSpeeds({m_frontLeft.GetState(), m_frontRight.GetState(), m_backLeft.GetState(), m_backRight.GetState()}); }, // ChassisSpeeds supplier. MUST BE ROBOT RELATIVE
        [this](frc::ChassisSpeeds speeds){Drive(speeds.vx, speeds.vy, speeds.omega, false, 0.02_s); }, // Method that will drive the robot given ROBOT RELATIVE ChassisSpeeds
        HolonomicPathFollowerConfig( // HolonomicPathFollowerConfig, this should likely live in your Constants class
            PIDConstants(KDriveSlot0kP, KDriveSlot0kI, KDriveSlot0kD), // Translation PID constants
            PIDConstants(KAngleSlot0kP, KAngleSlot0kI, KAngleSlot0kD), // Rotation PID constants
            kMaxSpeed, // Max module speed, in m/s
            kRobotRadius, // Drive base radius in meters. Distance from robot center to furthest module.
            ReplanningConfig() // Default path replanning config. See the API for the options here
        ),
        []() {
            // Boolean supplier that controls when the path will be mirrored for the red alliance
            // This will flip the path being followed to the red side of the field.
            // THE ORIGIN WILL REMAIN ON THE BLUE SIDE

            auto alliance = frc::DriverStation::GetAlliance();
            if (alliance) {
                return alliance.value() == frc::DriverStation::Alliance::kRed;
            }
            return false;
        },
        this // Reference to this subsystem to set requirements
    );
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

  SwerveModule m_frontLeft{kFrontLeftTurningMotorID, kFrontLeftDriveMotorID, kFrontLeftCancoderID, kFrontLeftAngleOffset};
  SwerveModule m_frontRight{kFrontRightTurningMotorID, kFrontRightDriveMotorID, kFrontRightCancoderID, kFrontRightAngleOffset};
  SwerveModule m_backLeft{kBackLeftTurningMotorID, kBackLeftDriveMotorID, kBackLeftCancoderID, kBackLeftAngleOffset};
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
