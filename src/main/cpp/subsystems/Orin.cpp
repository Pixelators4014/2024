// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <arpa/inet.h> // Include for inet_pton
#include <frc/smartdashboard/SmartDashboard.h>
#include <iostream>
#include <sys/socket.h>

#include "subsystems/Orin.h"

Orin::Orin() {
  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    std::cerr << "Socket creation failed" << std::endl;
  }

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
  servaddr.sin_port = htons(PORT);

  // Bind the socket with the server address
  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    std::cerr << "Bind failed" << std::endl;
  }

  std::cout << "Listening on port " << PORT << "..." << std::endl;
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

int Orin::getPose() {
  uint8_t request[1];
  request[0] = 0;
  // Send the request to the server
  sendto(sockfd, (const char *)request, sizeof(request), MSG_CONFIRM,
         (const struct sockaddr *)&servaddr, sizeof(servaddr));

  // Receive the response from the server
  int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL,
                   (struct sockaddr *)&cliaddr, &len);
  if (n < 0) {
    std::cerr << "Receive failed" << std::endl;
    return 254;
  }
  buffer[n] = '\0'; // Null terminate the string
  if (n < 25) {
    std::cerr << "Not enough data received" << std::endl;
    return 255;
  }
  if (buffer[0] == 1) {
    std::cerr << "Server error" << std::endl;
    return 1;
  } // TODO: Turn into string
  if (buffer[0] != 255) {
    std::cerr << "Invalid response" << std::endl;
    return 254;
  }
  float x, y, z, roll, pitch, yaw;
  memcpy(&x, buffer + 1, sizeof(float));
  memcpy(&y, buffer + 5, sizeof(float));
  memcpy(&z, buffer + 9, sizeof(float));
  memcpy(&roll, buffer + 13, sizeof(float));
  memcpy(&pitch, buffer + 17, sizeof(float));
  memcpy(&yaw, buffer + 21, sizeof(float));
  pose.x = x;
  pose.y = y;
  pose.z = z;
  pose.roll = roll;
  pose.pitch = pitch;
  pose.yaw = yaw;
  return 0;
}

int Orin::setPose(Pose pose) {
  uint8_t request[25];
  request[0] = 255;
  memcpy(request + 1, &pose.x, sizeof(float));
  memcpy(request + 5, &pose.y, sizeof(float));
  memcpy(request + 9, &pose.z, sizeof(float));
  memcpy(request + 13, &pose.roll, sizeof(float));
  memcpy(request + 17, &pose.pitch, sizeof(float));
  memcpy(request + 21, &pose.yaw, sizeof(float));
  // Send the request to the server
  sendto(sockfd, (const char *)request, sizeof(request), MSG_CONFIRM,
         (const struct sockaddr *)&servaddr, sizeof(servaddr));
  // Receive the response from the server
  int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL,
                   (struct sockaddr *)&cliaddr, &len);
  if (n < 0) {
    std::cerr << "Receive failed" << std::endl;
    return 1;
  }
  buffer[n] = '\0'; // Null terminate the string
  if (n < 1) {
    std::cerr << "Not enough data received" << std::endl;
    return 255;
  }
  if (buffer[0] != 0 && buffer[0] != 1) {
    std::cerr << "Invalid response" << std::endl;
    return 254;
  } // TODO: Turn `1` into string

  return buffer[0];
}
