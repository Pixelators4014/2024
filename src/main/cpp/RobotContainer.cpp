// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"
#include <frc/MathUtil.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/Commands.h>
#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/RunCommand.h>
#include <frc2/command/button/JoystickButton.h>
#include <iostream>
#include <memory>
#include <pathplanner/lib/auto/AutoBuilder.h>
#include <pathplanner/lib/auto/NamedCommands.h>
#include <pathplanner/lib/commands/PathPlannerAuto.h>
#include <pathplanner/lib/path/PathPlannerPath.h>

using namespace pathplanner;

RobotContainer::RobotContainer() {
  // Register named commands
  // NamedCommands::registerCommand("marker1", frc2::cmd::Print("Passed marker
  // 1")); NamedCommands::registerCommand("marker2", frc2::cmd::Print("Passed
  // marker 2")); NamedCommands::registerCommand("print hello",
  // frc2::cmd::Print("hello"));
  NamedCommands::registerCommand(
      "extendArm",
      std::make_shared<frc2::FunctionalCommand>(frc2::FunctionalCommand(
          [this] {}, [this] { m_arm.SetDesiredPosition(kArmUpTurns); },
          [this](bool interrupted) {},
          [this] {
            return m_arm.GetPosition() > (kArmUpTurns - kArmTurnTolerance);
          },
          {&m_arm})));
  NamedCommands::registerCommand(
      "retractArm",
      std::make_shared<frc2::FunctionalCommand>(frc2::FunctionalCommand(
          [this] {}, [this] { m_arm.SetDesiredPosition(kArmDownTurns); },
          [this](bool interrupted) {},
          [this] {
            return m_arm.GetPosition() < (kArmDownTurns + kArmTurnTolerance);
          },
          {&m_arm})));
  NamedCommands::registerCommand(
      "moveGrabber",
      std::make_shared<frc2::FunctionalCommand>(frc2::FunctionalCommand(
          [this] { m_grabber.ResetPosition(); },
          [this] { m_grabber.SetSpeed(kGrabberSpeed); },
          [this](bool interrupted) {},
          [this] { return m_grabber.GetPosition() > kGrabberScoreTurns; },
          {&m_grabber})));
  NamedCommands::registerCommand(
      "intakeGrabber",
      std::make_shared<frc2::FunctionalCommand>(frc2::FunctionalCommand(
          [this] {}, [this] { m_grabber.SetSpeed(kGrabberSpeed); },
          [this](bool interrupted) {},
          [this] { return m_grabber.IsFullyInserted(); }, {&m_grabber})));
  m_swerve.SetDefaultCommand(frc2::RunCommand(
      [this] {
        const auto xSpeed =
            -frc::ApplyDeadband(m_controller.GetY(), KDeadband) * kMaxSpeed;
        //  const auto xSpeed = m_controller.GetY()*
        //                 kMaxSpeed;

        // Get the y speed or sideways/strafe speed. We are inverting this
        // because we want a positive value when we pull to the left. Xbox
        // controllers return positive values when you pull to the right by
        // default.
        const auto ySpeed =
            -frc::ApplyDeadband(m_controller.GetX(), KDeadband) * kMaxSpeed;
        // const auto ySpeed = -m_controller.GetX() *
        //                     kMaxSpeed;

        // Get the rate of angular rotation. We are inverting this because we
        // want a positive value when we pull to the left (remember, CCW is
        // positive in mathematics). Xbox controllers return positive values
        // when you pull to the right by default.
        const auto rot =
            -frc::ApplyDeadband(m_controller.GetRawAxis(5), KDeadband) *
            kMaxAngularSpeed;
        // const auto rot = -m_controller.GetZ() *
        //                  kMaxAngularSpeed;
        m_swerve.driveFieldRelative(frc::ChassisSpeeds{xSpeed, ySpeed, rot});
      },
      {&m_swerve}));

  m_arm.SetDefaultCommand(frc2::RunCommand(
      [this] {
        // Get the z axis of the controller, then make 1 go to kArmDownTurns and
        // -1 go to kArmUpTurns
        m_arm.SetDesiredPosition((-m_controller.GetZ() + 1.0) / 2.0 *
                                     (kArmUpTurns - kArmDownTurns) +
                                 kArmDownTurns);
      },
      {&m_arm}));

  m_grabber.SetDefaultCommand(frc2::RunCommand(
      [this] {
        if (m_controller.GetRawButton(kGrabberAutoIntakeButton) &&
            !m_grabber.IsFullyInserted()) {
          m_grabber.SetSpeed(kGrabberSpeed);
        }
        // else if (m_controller.GetRawButton(kGrabberOutakeButton)) {
        //   m_grabber.SetSpeed(-kGrabberSpeed);
        // } else if (m_controller.GetRawButton(kGrabberButton)) {
        //   m_grabber.SetSpeed(kGrabberSpeed);
        // }
        else {
          m_grabber.SetSpeed(kGrabberSpeed * m_controller.GetRawAxis(6));
        }
      },
      {&m_grabber}));

  // Configure the button bindings
  ConfigureBindings();
}

void RobotContainer::ConfigureBindings() {
  // frc2::JoystickButton(&m_controller,
  // kMoveToAmpButton).OnTrue(frc::cmd::Run([this] {}));

  // frc2::JoystickButton(&m_controller, kIntakeDownButton)
  //     .OnTrue(
  //         frc2::cmd::Run([this] { m_arm.SetDesiredPosition(kArmDownTurns);
  //         }));
  // frc2::JoystickButton(&m_controller, kIntakeUpButton)
  //     .OnTrue(
  //         frc2::cmd::Run([this] { m_arm.SetDesiredPosition(kArmUpTurns); }));
  // frc2::JoystickButton(&m_controller, kGrabberButton)
  //     .OnTrue(frc2::cmd::RunOnce([this] { m_grabber.SetSpeed(kGabberSpeed);
  //     }));
  // frc2::JoystickButton(&m_controller, kGrabberButton)
  //     .OnFalse(frc2::cmd::RunOnce([this] { m_grabber.SetSpeed(0.0); }));
  // frc2::JoystickButton(&m_controller, kGrabberOutakeButton)
  //     .OnTrue(
  //         frc2::cmd::RunOnce([this] { m_grabber.SetSpeed(-kGabberSpeed); }));
  // frc2::JoystickButton(&m_controller, kGrabberButton)
  //     .OnFalse(frc2::cmd::RunOnce([this] { m_grabber.SetSpeed(0.0); }));
  // frc2::JoystickButton(&m_controller, kGrabberAutoIntakeButton)
  //     .WhileTrue(frc2::cmd::Run([this] {
  //       if (!m_grabber.IsFullyInserted())
  //         m_grabber.SetSpeed(-kGabberSpeed);
  //       else
  //         m_grabber.SetSpeed(0.0);
  //     }));
  // frc2::JoystickButton(&m_controller, kGrabberButton)
  //     .OnFalse(frc2::cmd::RunOnce([this] { m_grabber.SetSpeed(0.0); }));
  // // Add a button to run the example auto to SmartDashboard, this will also
  // be in the GetAutonomousCommand method below exampleAuto =
  // PathPlannerAuto("Example Auto").ToPtr().Unwrap();
  // frc::SmartDashboard::PutData("Example Auto", exampleAuto.get());

  // // Add a button to run pathfinding commands to SmartDashboard
  // pathfindToPickup = AutoBuilder::pathfindToPose(
  //                        frc::Pose2d(14.0_m, 6.5_m, frc::Rotation2d(0_deg)),
  //                        PathConstraints(4.0_mps, 4.0_mps_sq, 360_deg_per_s,
  //                        540_deg_per_s_sq), 0_mps, 2.0_m) .Unwrap();
  // frc::SmartDashboard::PutData("Pathfind to Pickup Pos",
  // pathfindToPickup.get()); pathfindToScore = AutoBuilder::pathfindToPose(
  //                       frc::Pose2d(2.15_m, 3.0_m, frc::Rotation2d(180_deg)),
  //                       PathConstraints(4.0_mps, 4.0_mps_sq, 360_deg_per_s,
  //                       540_deg_per_s_sq), 0_mps, 0_m) .Unwrap();
  // frc::SmartDashboard::PutData("Pathfind to Scoring Pos",
  // pathfindToScore.get());

  // // Add a button to SmartDashboard that will create and follow an on-the-fly
  // path
  // // This example will simply move the robot 2m in the +X field direction
  // onTheFly = frc2::cmd::RunOnce([this]()
  //                               {
  // frc::Pose2d currentPose = m_swerve.GetPose();

  // // The rotation component in these poses represents the direction of travel
  // frc::Pose2d startPos = frc::Pose2d(currentPose.Translation(),
  // frc::Rotation2d()); frc::Pose2d endPos =
  // frc::Pose2d(currentPose.Translation() + frc::Translation2d(2.0_m, 0_m),
  // frc::Rotation2d());

  // std::vector<frc::Translation2d> bezierPoints =
  // PathPlannerPath::bezierFromPoses({startPos, endPos});
  // // Paths must be used as shared pointers
  // auto path = std::make_shared<PathPlannerPath>(
  //   bezierPoints,
  //   PathConstraints(4.0_mps, 4.0_mps_sq, 360_deg_per_s, 540_deg_per_s_sq),
  //   GoalEndState(0_mps, currentPose.Rotation())
  // );

  // // Prevent this path from being flipped on the red alliance, since the
  // given positions are already correct path->preventFlipping = true;

  // this->followOnTheFly = AutoBuilder::followPath(path).Unwrap();
  // this->followOnTheFly->Schedule(); })
  //                .Unwrap();
  // frc::SmartDashboard::PutData("On-the-fly path", onTheFly.get());
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand() {
  return PathPlannerAuto("Blue Left").ToPtr();
  // return frc2::cmd::RunOnce([this] {});
}
