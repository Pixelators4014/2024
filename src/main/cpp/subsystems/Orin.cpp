// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

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
    servaddr.sin_family = AF_INET;         // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
    }

    std::cout << "Listening on port " << PORT << "..." << std::endl;
}

void Orin::Periodic() {
    int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL,
                     (struct sockaddr *)&cliaddr, &len);
    if (n < 0) {
        std::cerr << "Receive failed" << std::endl;
        return;
    }
    buffer[n] = '\0'; // Null terminate the string

    if (n < 32) {
        std::cerr << "Not enough data received" << std::endl;
        return;
    }

    unsigned header;
    float x, y, z, theta_w, theta_x, theta_y, theta_z;
    memcpy(&header, buffer, sizeof(unsigned));
    memcpy(&x, buffer + 4, sizeof(float));
    memcpy(&y, buffer + 8, sizeof(float));
    memcpy(&z, buffer + 12, sizeof(float));
    memcpy(&theta_w, buffer + 16, sizeof(float));
    memcpy(&theta_x, buffer + 20, sizeof(float));
    memcpy(&theta_y, buffer + 24, sizeof(float));
    memcpy(&theta_z, buffer + 28, sizeof(float));

    std::cout << "Received: "
              << "x=" << x << ", y=" << y << ", z=" << z << ", theta_w=" << theta_w
              << ", theta_x=" << theta_x << ", theta_y=" << theta_y << ", theta_z=" << theta_z
              << std::endl;
    // Update the SmartDashboard values
    frc::SmartDashboard::PutNumber("x", x);
    frc::SmartDashboard::PutNumber("y", y);
    frc::SmartDashboard::PutNumber("z", z);
    frc::SmartDashboard::PutNumber("theta_w", theta_w);
    frc::SmartDashboard::PutNumber("theta_x", theta_x);
    frc::SmartDashboard::PutNumber("theta_y", theta_y);
    frc::SmartDashboard::PutNumber("theta_z", theta_z);
}
