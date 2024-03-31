// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <frc/smartdashboard/SmartDashboard.h>
#include <iostream>

#include "subsystems/Orin.h"

Orin::Orin() : socket{kn::endpoint(SERVER_TARGET_IP, SERVER_TARGET_PORT)} {
  socket.bind();
}

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
  for (unsigned char i = 0; i < BUFFER_SIZE; i++)
    buff[0] = std::byte{0};
  socket.send(buff.data(), BUFFER_SIZE);
  auto [received_bytes, status] = socket.recv(buff);
  const auto from = socket.get_recv_endpoint();
  if (received_bytes != 25) {
    std::cout << "HELP ME, NOT ENOUGH DATA!! I got" << received_bytes
              << " bytes" << std::endl;
  }
  float x;
  float y;
  float z;
  float roll;
  float pitch;
  float yaw;
  memcpy(&x, &buff[1], sizeof(float));
  memcpy(&y, &buff[5], sizeof(float));
  memcpy(&z, &buff[9], sizeof(float));
  memcpy(&roll, &buff[13], sizeof(float));
  memcpy(&pitch, &buff[17], sizeof(float));
  memcpy(&yaw, &buff[21], sizeof(float));
  pose.x = x;
  pose.y = y;
  pose.z = z;
  pose.roll = roll;
  pose.pitch = pitch;
  pose.yaw = yaw;
}
