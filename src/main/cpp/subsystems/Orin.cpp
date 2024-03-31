// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <frc/smartdashboard/SmartDashboard.h>

#include "subsystems/Orin.h"

Orin::Orin() : a_socket{kn::endpoint(SERVER_TARGET_IP, SERVER_TARGET_PORT)} {}

void Orin::Periodic() {
  getPose();
  // frc::SmartDashboard::PutNumber("x", pose.x);
  // frc::SmartDashboard::PutNumber("y", pose.y);
  // frc::SmartDashboard::PutNumber("z", pose.z);
  // frc::SmartDashboard::PutNumber("roll", pose.roll);
  // frc::SmartDashboard::PutNumber("pitch", pose.pitch);
  // frc::SmartDashboard::PutNumber("yaw", pose.yaw);
}

void Orin::getPose() {
  kn::buffer<BUFFER_SIZE> buff;
  for (unsigned char i = 0; i < BUFFER_SIZE; i++)
    buff[i] = std::byte{i};
  a_socket.send(buff.data(), BUFFER_SIZE);
  // float x, y, z, roll, pitch, yaw;
  // memcpy(&x, buffer + 1, sizeof(float));
  // memcpy(&y, buffer + 5, sizeof(float));
  // memcpy(&z, buffer + 9, sizeof(float));
  // memcpy(&roll, buffer + 13, sizeof(float));
  // memcpy(&pitch, buffer + 17, sizeof(float));
  // memcpy(&yaw, buffer + 21, sizeof(float));
  // pose.x = x;
  // pose.y = y;
  // pose.z = z;
  // pose.roll = roll;
  // pose.pitch = pitch;
  // pose.yaw = yaw;
}
