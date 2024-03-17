// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystems/Drivetrain.h"
#include <frc/DriverStation.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <pathplanner/lib/auto/AutoBuilder.h>
#include <pathplanner/lib/util/HolonomicPathFollowerConfig.h>
#include <pathplanner/lib/util/PIDConstants.h>
#include <pathplanner/lib/util/PathPlannerLogging.h>
#include <pathplanner/lib/util/ReplanningConfig.h>

using namespace pathplanner;

Drivetrain::Drivetrain() {
  m_IMU.Reset();
  AutoBuilder::configureHolonomic(
      [this]() { return GetPose(); }, // Robot pose supplier
      [this](frc::Pose2d pose) {
        ResetPose(pose);
      }, // Method to reset odometry (will be called if your auto has a starting
         // pose)
      [this]() {
        return GetSpeeds();
      }, // ChassisSpeeds supplier. MUST BE ROBOT RELATIVE
      [this](frc::ChassisSpeeds speeds) {
        DriveRobotRelative(speeds);
      }, // Method that will drive the robot given ROBOT RELATIVE ChassisSpeeds
      HolonomicPathFollowerConfig( // HolonomicPathFollowerConfig, this should
                                   // likely live in your Constants class
          PIDConstants(KDriveSlot0kP, KDriveSlot0kI,
                       KDriveSlot0kD), // Translation PID constants
          PIDConstants(KAngleSlot0kP, KAngleSlot0kI,
                       KAngleSlot0kD), // Rotation PID constants
          kMaxSpeed,                   // Max module speed, in m/s
          kRobotRadius,      // Drive base radius in meters. Distance from robot
                             // center to furthest module.
          ReplanningConfig() // Default path replanning config. See the API for
                             // the options here
          ),
      []() {
        // Boolean supplier that controls when the path will be mirrored for the
        // red alliance This will flip the path being followed to the red side
        // of the field. THE ORIGIN WILL REMAIN ON THE BLUE SIDE

        auto alliance = frc::DriverStation::GetAlliance();
        if (alliance) {
          return alliance.value() == frc::DriverStation::Alliance::kRed;
        }
        return false;
      },
      this // Reference to this subsystem to set requirements
  );

  PathPlannerLogging::setLogActivePathCallback(
      [this](auto poses) { this->field.GetObject("path")->SetPoses(poses); });

  frc::SmartDashboard::PutData("Field", &field);
}

void Drivetrain::Periodic() {
  UpdateOdometry();

  field.SetRobotPose(GetPose());
}

void Drivetrain::DriveRobotRelative(
    const frc::ChassisSpeeds &robotRelativeSpeeds) {
  auto states = m_kinematics.ToSwerveModuleStates(
      frc::ChassisSpeeds::Discretize(robotRelativeSpeeds, 0.02_s));

  m_kinematics.DesaturateWheelSpeeds(&states, kMaxSpeed);

  auto [fl, fr, bl, br] = states;

  m_frontLeft.SetDesiredState(fl);
  m_frontRight.SetDesiredState(fr);
  m_backLeft.SetDesiredState(bl);
  m_backRight.SetDesiredState(br);
}

void Drivetrain::UpdateOdometry() {
  m_odometry.Update(m_IMU.GetAngle(),
                    {m_frontLeft.GetPosition(), m_frontRight.GetPosition(),
                     m_backLeft.GetPosition(), m_backRight.GetPosition()});
}
