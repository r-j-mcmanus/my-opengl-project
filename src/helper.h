#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


inline  glm::mat4 createRotationMatrix(const glm::vec3& rotation) {
    // TODO quaterions 
    // Convert degrees to radians
    float pitchRad = glm::radians(rotation[0]);
    float yawRad = glm::radians(rotation[1]);
    float rollRad = glm::radians(rotation[2]);

    // Rotation around X-axis (pitch)
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), pitchRad, glm::vec3(1.0f, 0.0f, 0.0f));

    // Rotation around Y-axis (yaw)
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), yawRad, glm::vec3(0.0f, 1.0f, 0.0f));

    // Rotation around Z-axis (roll)
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), rollRad, glm::vec3(0.0f, 0.0f, 1.0f));

    // Combine rotations (order: ZYX)
    glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;

    return rotationMatrix;
}