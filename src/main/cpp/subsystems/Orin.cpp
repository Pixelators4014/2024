// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <frc/smartdashboard/SmartDashboard.h>
#include <iostream>

#include "subsystems/Orin.h"

Orin::Orin() : socket{kn::endpoint(SERVER_TARGET_IP, SERVER_TARGET_PORT)} {}

void Orin::Periodic() {
  getPose();
  frc::SmartDashboard::PutNumber("x", pose.x);
  frc::SmartDashboard::PutNumber("y", pose.y);
  frc::SmartDashboard::PutNumber("z", pose.z);
  frc::SmartDashboard::PutNumber("roll", pose.roll);
  frc::SmartDashboard::PutNumber("pitch", pose.pitch);
  frc::SmartDashboard::PutNumber("yaw", pose.yaw);
}

void Orin::getPose() {
  unsigned char msg[1] = {0};
  std::cout << "Sending GetPose request" << std::endl;
  socket.send(reinterpret_cast<const std::byte *>(msg), 1);
  kn::buffer<4096> static_buffer;
  if (socket.bytes_available() >= 25) {
    std::cout << "Received GetPose response" << std::endl;
    const auto [data_size, status_code] = socket.recv(static_buffer);
    const unsigned int *arr =
        reinterpret_cast<const unsigned int *>(static_buffer.data());
    if (arr[0] != 255) {
      std::cout << "Invalid response" << std::endl;
      return;
    }
    float x;
    float y;
    float z;
    float roll;
    float pitch;
    float yaw;
    memcpy(&x, &arr[1], sizeof(float));
    memcpy(&y, &arr[5], sizeof(float));
    memcpy(&z, &arr[9], sizeof(float));
    memcpy(&roll, &arr[13], sizeof(float));
    memcpy(&pitch, &arr[17], sizeof(float));
    memcpy(&yaw, &arr[21], sizeof(float));
    pose.x = x;
    pose.y = y;
    pose.z = z;
    pose.roll = roll;
    pose.pitch = pitch;
    pose.yaw = yaw;
  }
}
