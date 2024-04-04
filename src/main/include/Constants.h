#include <ctre/phoenix6/TalonFX.hpp>
#include <units/velocity.h>

#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1
static constexpr auto KSlewRate = 3 / 1_s;
static constexpr auto KDeadband = 0.05;

static constexpr auto KCancoderSensorDirection =
    ctre::phoenix6::signals::SensorDirectionValue::CounterClockwise_Positive;
static constexpr auto KDriveMotorInverted =
    ctre::phoenix6::signals::InvertedValue::Clockwise_Positive;
static constexpr auto KDriveMotorNeutralMode =
    ctre::phoenix6::signals::NeutralModeValue::Coast;
static constexpr auto KDriveSensorToMechanismRatio = 6.12;
static constexpr auto KDriveCurrentLimitEnable = true;
static constexpr auto KDriveCurrentLimit = 60;
static constexpr auto KDriveCurrentThreshold = 35;
static constexpr auto KDriveTimeThreshold = 0.1;
static constexpr auto KDriveSlot0kP = 1.0;
static constexpr auto KDriveSlot0kI = 0.0;
static constexpr auto KDriveSlot0kD = 0.0;
static constexpr auto KDriveDutyCycleOpenLoopRampPeriod = 0.1;
static constexpr auto KDriveVoltageOpenLoopRampPeriod = 0.1;
static constexpr auto KDriveDutyCycleClosedLoopRampPeriod = 0.1;
static constexpr auto KDriveVoltageClosedLoopRampPeriod = 0.1;
static constexpr auto KAngleMotorInverted =
    ctre::phoenix6::signals::InvertedValue::Clockwise_Positive;
static constexpr auto KAngleSensorToMechanismRatio = 150.0 / 7.0;
static constexpr auto KAngleContinuousWrap = true;
static constexpr auto KAngleCurrentLimitEnable = true;
static constexpr auto KAngleCurrentLimit = 40;
static constexpr auto KAngleCurrentThreshold = 25;
static constexpr auto KAngleTimeThreshold = 0.1;
static constexpr auto KAngleSlot0kP = 24.0;
static constexpr auto KAngleSlot0kI = 0.0;
static constexpr auto KAngleSlot0kD = 0.0;

static constexpr auto KDrivePathfinder0kP = 5.0;
static constexpr auto KDrivePathfinder0kI = 0.0;
static constexpr auto KDrivePathfinder0kD = 0.0;

static constexpr auto KAnglePathfinder0kP = 5.0;
static constexpr auto KAnglePathfinder0kI = 0.0;
static constexpr auto KAnglePathfinder0kD = 0.0;

static constexpr auto kMaxSpeed = 5.2_mps;
static constexpr units::radians_per_second_t kMaxAngularSpeed{std::numbers::pi};
static constexpr auto kRobotRadius = 0.381_m;
static constexpr auto kWheelRadius = 0.0508_m;
static constexpr auto kTurnsToMeters =
    2 * std::numbers::pi * kWheelRadius / 1_tr;

static constexpr auto kFieldRelative = true;

static constexpr auto kControllerPort = 0;

static constexpr auto kBackLeftTurningMotorID = 1;
static constexpr auto kBackLeftDriveMotorID = 2;
static constexpr auto kBackLeftCancoderID = 11;
static constexpr auto kBackLeftAngleOffset = -0.18_tr;
static constexpr auto kFrontLeftTurningMotorID = 3;
static constexpr auto kFrontLeftDriveMotorID = 4;
static constexpr auto kFrontLeftCancoderID = 12;
static constexpr auto kFrontLeftAngleOffset = -0.31_tr;
static constexpr auto kFrontRightTurningMotorID = 5;
static constexpr auto kFrontRightDriveMotorID = 6;
static constexpr auto kFrontRightCancoderID = 13;
static constexpr auto kFrontRightAngleOffset = 0.262_tr;
static constexpr auto kBackRightTurningMotorID = 7;
static constexpr auto kBackRightDriveMotorID = 8;
static constexpr auto kBackRightCancoderID = 14;
static constexpr auto kBackRightAngleOffset = -0.223_tr;

static constexpr auto kLeftArmMotorID = 21;
static constexpr auto kRightArmMotorID = 22;
static constexpr auto kArmGearRatio = 35.0; // TODO: Calculate this
static constexpr auto kArmUpTurns = 115_tr / 360;
static constexpr auto kArmDownTurns = 0.05_tr;
static constexpr auto kLeftArmMotorInverted =
    ctre::phoenix6::signals::InvertedValue::Clockwise_Positive;
static constexpr auto kRightArmMotorInverted =
    ctre::phoenix6::signals::InvertedValue::CounterClockwise_Positive;
static constexpr auto kArmSlot0kP = 12.0;
static constexpr auto kArmSlot0kI = 0.0;
static constexpr auto kArmSlot0kD = 0.0;
static constexpr auto kArmNeutralMode =
    ctre::phoenix6::signals::NeutralModeValue::Brake;
static constexpr auto kArmCurrentLimitEnable = true;
static constexpr auto kArmCurrentLimit = 40;
static constexpr auto kArmCurrentThreshold = 25;
static constexpr auto kArmTimeThreshold = 0.1;
static constexpr auto kArmDutyCycleOpenLoopRampPeriod = 0.1;
static constexpr auto kArmVoltageOpenLoopRampPeriod = 0.1;
static constexpr auto kArmDutyCycleClosedLoopRampPeriod = 0.1;
static constexpr auto kArmVoltageClosedLoopRampPeriod = 0.1;

static constexpr auto kMoveToAmpButton = 4;
static constexpr auto kGrabberButton = 3;
static constexpr auto kGrabberOutakeButton = 2;
static constexpr auto kGrabberAutoIntakeButton = 1;
static constexpr auto kGrabberMotorID = 31;
static constexpr auto kGrabberSpeed = 0.5;
static constexpr auto KGrabberMotorInverted =
    ctre::phoenix6::signals::InvertedValue::Clockwise_Positive;
static constexpr auto kLowerBreakBeamID = 0;
static constexpr auto kUpperBreakBeamID = 3;
#endif
