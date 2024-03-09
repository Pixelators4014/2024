#include <units/acceleration.h>
#include <units/angle.h>
#include <units/angular_acceleration.h>
#include <units/angular_velocity.h>
#include <units/area.h>
#include <units/capacitance.h>
#include <units/charge.h>
#include <units/concentration.h>
#include <units/conductance.h>
#include <units/current.h>
#include <units/curvature.h>
#include <units/data.h>
#include <units/data_transfer_rate.h>
#include <units/density.h>
#include <units/dimensionless.h>
#include <units/energy.h>
#include <units/force.h>
#include <units/frequency.h>
#include <units/illuminance.h>
#include <units/impedance.h>
#include <units/inductance.h>
#include <units/length.h>
#include <units/luminous_flux.h>
#include <units/luminous_intensity.h>
#include <units/magnetic_field_strength.h>
#include <units/magnetic_flux.h>
#include <units/mass.h>
#include <units/moment_of_inertia.h>
#include <units/power.h>
#include <units/pressure.h>
#include <units/radiation.h>
#include <units/solid_angle.h>
#include <units/substance.h>
#include <units/temperature.h>
#include <units/time.h>
#include <units/torque.h>
#include <units/velocity.h>
#include <units/voltage.h>
#include <units/volume.h>
#include <units/constants.h>
#include <ctre/phoenix6/CANcoder.hpp>
#include <ctre/phoenix6/TalonFX.hpp>

static constexpr auto KSlewRate = 3 / 1_s;
static constexpr auto KDeadband = 0.05;

static constexpr auto KCancoderSensorDirection = ctre::phoenix6::signals::SensorDirectionValue::CounterClockwise_Positive;
static constexpr auto KDriveMotorInverted = ctre::phoenix6::signals::InvertedValue::Clockwise_Positive;
static constexpr auto KDriveMotorNeutralMode = ctre::phoenix6::signals::NeutralModeValue::Coast;
static constexpr auto KDriveSensorToMechanismRatio = 6.12;
static constexpr auto KDriveCurrentLimitEnable = true;
static constexpr auto KDriveCurrentLimit = 60;
static constexpr auto KDriveCurrentThreshold = 50;
static constexpr auto KDriveTimeThreshold = 0.1;
static constexpr auto KDriveSlot0kP = 1.0;
static constexpr auto KDriveSlot0kI = 0.0;
static constexpr auto KDriveSlot0kD = 0.0;
static constexpr auto KDriveDutyCycleOpenLoopRampPeriod = 0.1;
static constexpr auto KDriveVoltageOpenLoopRampPeriod = 0.1;
static constexpr auto KDriveDutyCycleClosedLoopRampPeriod = 0.1;
static constexpr auto KDriveVoltageClosedLoopRampPeriod = 0.1;
static constexpr auto KAngleMotorInverted = ctre::phoenix6::signals::InvertedValue::Clockwise_Positive;
static constexpr auto KAngleSensorToMechanismRatio = 150.0 / 7.0;
static constexpr auto KAngleContinuousWrap = true;
static constexpr auto KAngleCurrentLimitEnable = true;
static constexpr auto KAngleCurrentLimit = 40;
static constexpr auto KAngleCurrentThreshold = 25;
static constexpr auto KAngleTimeThreshold = 0.1;
static constexpr auto KAngleSlot0kP = 24.0;
static constexpr auto KAngleSlot0kI = 0.0;
static constexpr auto KAngleSlot0kD = 0.0;

static constexpr auto kMaxSpeed =
      3.0_mps;
static constexpr units::radians_per_second_t kMaxAngularSpeed{
    std::numbers::pi};
static constexpr auto kRobotRadius = 0.381_m;
static constexpr auto kWheelRadius = 0.0508_m;
static constexpr auto kTurnsToMeters = 2 * std::numbers::pi * kWheelRadius / 1_tr;

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

static constexpr auto EnableFOC = false;