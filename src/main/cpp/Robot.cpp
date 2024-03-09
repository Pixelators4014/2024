// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <frc/MathUtil.h>
#include <frc/TimedRobot.h>
#include <frc/XboxController.h>
#include <frc/Joystick.h>
#include <frc/filter/SlewRateLimiter.h>

#include "Drivetrain.h"

class Robot : public frc::TimedRobot
{
public:
  void AutonomousPeriodic() override
  {
    // DriveWithJoystick(true);
    // m_swerve.UpdateOdometry();
  }

  void TeleopPeriodic() override { DriveWithJoystick(kFieldRelative); }

private:
  // frc::XboxController m_controller{0};
  frc::Joystick m_controller{kControllerPort};
  Drivetrain m_swerve;

  // Slew rate limiters to make joystick inputs more gentle; 1/3 sec from 0
  // to 1.
  frc::SlewRateLimiter<units::scalar> m_xspeedLimiter{KSlewRate};
  frc::SlewRateLimiter<units::scalar> m_yspeedLimiter{KSlewRate};
  frc::SlewRateLimiter<units::scalar> m_rotLimiter{KSlewRate};

  void DriveWithJoystick(bool fieldRelative)
  {
    // Get the x speed. We are inverting this because Xbox controllers return
    // negative values when we push forward.
    const auto xSpeed = -m_xspeedLimiter.Calculate(
                            frc::ApplyDeadband(m_controller.GetY(), KDeadband)) *
                        kMaxSpeed;

    // Get the y speed or sideways/strafe speed. We are inverting this because
    // we want a positive value when we pull to the left. Xbox controllers
    // return positive values when you pull to the right by default.
    const auto ySpeed = -m_yspeedLimiter.Calculate(
                            frc::ApplyDeadband(m_controller.GetX(), KDeadband)) *
                        kMaxSpeed;

    // Get the rate of angular rotation. We are inverting this because we want a
    // positive value when we pull to the left (remember, CCW is positive in
    // mathematics). Xbox controllers return positive values when you pull to
    // the right by default.
    const auto rot = -m_rotLimiter.Calculate(
                         frc::ApplyDeadband(m_controller.GetZ(), KDeadband)) *
                     kMaxAngularSpeed;

    m_swerve.Drive(xSpeed, ySpeed, rot, fieldRelative, GetPeriod());
  }
};

#ifndef RUNNING_FRC_TESTS
int main()
{
  return frc::StartRobot<Robot>();
}
#endif
