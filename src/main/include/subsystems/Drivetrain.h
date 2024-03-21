// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/ADIS16470_IMU.h>
#include <frc/geometry/Translation2d.h>
#include <frc/kinematics/SwerveDriveKinematics.h>
#include <frc/kinematics/SwerveDriveOdometry.h>
#include <frc/smartdashboard/Field2d.h>
#include <frc2/command/SubsystemBase.h>

#include "Constants.h"
#include "SwerveModule.h"

class Drivetrain : public frc2::SubsystemBase {
public:
    Drivetrain();

    void Periodic() override;

    inline const frc::Pose2d &GetPose() const { return m_odometry.GetPose(); }

    inline void ResetPose(const frc::Pose2d &pose) {
        m_odometry.ResetPosition(
                m_IMU.GetAngle(),
                {m_frontLeft.GetPosition(), m_frontRight.GetPosition(),
                 m_backLeft.GetPosition(), m_backRight.GetPosition()},
                pose);
    }

    inline const frc::ChassisSpeeds GetSpeeds() {
        return m_kinematics.ToChassisSpeeds(
                {m_frontLeft.GetState(), m_frontRight.GetState(), m_backLeft.GetState(),
                 m_backRight.GetState()});
    }

    void DriveRobotRelative(const frc::ChassisSpeeds &robotRelativeSpeeds);

    inline void
    driveFieldRelative(const frc::ChassisSpeeds &fieldRelativeSpeeds) {
        DriveRobotRelative(frc::ChassisSpeeds::FromFieldRelativeSpeeds(
                fieldRelativeSpeeds, GetPose().Rotation()));
    }

    void UpdateOdometry();

private:
    frc::Translation2d m_frontLeftLocation{kRobotRadius, kRobotRadius};
    frc::Translation2d m_frontRightLocation{kRobotRadius, -kRobotRadius};
    frc::Translation2d m_backLeftLocation{-kRobotRadius, kRobotRadius};
    frc::Translation2d m_backRightLocation{-kRobotRadius, -kRobotRadius};

    SwerveModule m_frontLeft{kFrontLeftTurningMotorID, kFrontLeftDriveMotorID,
                             kFrontLeftCancoderID, kFrontLeftAngleOffset};
    SwerveModule m_frontRight{kFrontRightTurningMotorID, kFrontRightDriveMotorID,
                              kFrontRightCancoderID, kFrontRightAngleOffset};
    SwerveModule m_backLeft{kBackLeftTurningMotorID, kBackLeftDriveMotorID,
                            kBackLeftCancoderID, kBackLeftAngleOffset};
    SwerveModule m_backRight{kBackRightTurningMotorID, kBackRightDriveMotorID,
                             kBackRightCancoderID, kBackRightAngleOffset};

    frc::ADIS16470_IMU m_IMU;

    frc::SwerveDriveKinematics<4> m_kinematics{
            m_frontLeftLocation, m_frontRightLocation, m_backLeftLocation,
            m_backRightLocation};

    frc::SwerveDriveOdometry<4> m_odometry{
            m_kinematics,
            m_IMU.GetAngle(),
            {m_frontLeft.GetPosition(), m_frontRight.GetPosition(),
             m_backLeft.GetPosition(), m_backRight.GetPosition()}};

    frc::Field2d field;
};
