// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <cstddef>
#include <frc/smartdashboard/SmartDashboard.h>
// #include <iostream>

#include "subsystems/Orin.h"

Orin::Orin() : socket{kn::endpoint(SERVER_TARGET_IP, SERVER_TARGET_PORT)} {
  send_buffer[0] = std::byte{0};
}

void Orin::Periodic() {
  // std::cout << "Sending GetPose request" << std::endl;
  socket.send(send_buffer);
  if (socket.bytes_available() >= 25) {
    // std::cout << "I see " << socket.bytes_available() << " avaliable bytes"
    //           << std::endl;
    const auto [data_size, status_code] = socket.recv(receive_buffer);
    // for (std::byte byte : static_buffer) {
    //   std::cout << static_cast<int>(byte) << ", ";
    // }
    // std::cout << std::endl;
    // if (static_buffer[0] != std::byte{255}) {
    //   std::cout << "Invalid response" << std::endl;
    //   return;
    // }
    float x;
    float y;
    float z;
    float roll;
    float pitch;
    float yaw;
    memcpy(&x, &receive_buffer[1], sizeof(float));
    memcpy(&y, &receive_buffer[5], sizeof(float));
    memcpy(&z, &receive_buffer[9], sizeof(float));
    memcpy(&roll, &receive_buffer[13], sizeof(float));
    memcpy(&pitch, &receive_buffer[17], sizeof(float));
    memcpy(&yaw, &receive_buffer[21], sizeof(float));
    pose.x = x;
    pose.y = y;
    pose.z = z;
    pose.roll = roll;
    pose.pitch = pitch;
    pose.yaw = yaw;
    frc::SmartDashboard::PutNumber("x", pose.x);
    frc::SmartDashboard::PutNumber("y", pose.y);
    frc::SmartDashboard::PutNumber("z", pose.z);
    frc::SmartDashboard::PutNumber("roll", pose.roll);
    frc::SmartDashboard::PutNumber("pitch", pose.pitch);
    frc::SmartDashboard::PutNumber("yaw", pose.yaw);
  }
}
