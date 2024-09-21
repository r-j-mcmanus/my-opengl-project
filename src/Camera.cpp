#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>         // For glm::value_ptr
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp> // todo replace with quats

#include <iostream>

#include "Transformer.h"
#include "EventListenerInterface.h"
#include "Camera.h"


Camera::Camera(const glm::vec3& position, const glm::vec3& viewDirection, const glm::vec3& up, float fov, float aspectRatio, float nearPlane, float farPlane)
    : Transformer(position), viewDirection(viewDirection), up(glm::normalize(up)), fov(fov), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane)
{
    updateViewMatrix();
    updateProjectionMatrix();
}

void Camera::setAspectRatio(const int width, const int height) {
    aspectRatio = (float)width / (float)height;
    updateProjectionMatrix();
}

void Camera::setPosition(const glm::vec3& newPosition) {
    Transformer::setPosition(newPosition);
    updateViewMatrix();
}

void Camera::setTarget(const glm::vec3& target) {
    viewDirection = glm::normalize(Transformer::getPosition() - target);
    updateViewMatrix(target);
}

void Camera::setAspectRatio(float newAspectRatio) {
    aspectRatio = newAspectRatio;
    updateProjectionMatrix();
}

glm::mat4 Camera::getViewMatrix() const {
    return viewMatrix;
}

glm::mat4 Camera::getDirectionMatrix() const {
    return glm::lookAt(glm::vec3(0, 0, 0), viewDirection, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return projectionMatrix;
}

void Camera::update(float dt) {
    Transformer::update(dt);
    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    glm::vec3 position = Transformer::getPosition();
    glm::vec3 target = position - viewDirection;

    viewMatrix = glm::lookAt(Transformer::getPosition(), target, up);
}

void Camera::updateViewMatrix(const glm::vec3& target) {
    viewMatrix = glm::lookAt(Transformer::getPosition(), target, up);
}

void Camera::updateProjectionMatrix() {
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

glm::vec3 Camera::getDirectionWithoutUp() const {
    // idea would be to get direction parrallel to the floor
    glm::vec3 projection = glm::dot(viewDirection, up) * up;
    glm::vec3 result = viewDirection - projection;

    return glm::normalize(result);

}

void Camera::onKeyEvent(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W) {
        glm::vec3 direction = getDirectionWithoutUp();
        if (action == GLFW_PRESS) {
            incrementVelocity(direction);
            return;
        }
        else if (action == GLFW_RELEASE) {
            incrementVelocity(-direction);
            return;
        }
    }

    if (key == GLFW_KEY_S) {
        glm::vec3 direction = getDirectionWithoutUp();
        if (action == GLFW_PRESS) {
            incrementVelocity(-direction);
            return;
        }
        else if (action == GLFW_RELEASE) {
            incrementVelocity(direction);
            return;
        }
    }

    if (key == GLFW_KEY_A) {
        glm::vec3 direction = getDirectionWithoutUp();
        glm::vec3 perp_direction = glm::cross(direction, up);
        if (action == GLFW_PRESS) {
            incrementVelocity(-perp_direction);
            return;
        }
        else if (action == GLFW_RELEASE) {
            incrementVelocity(perp_direction);
            return;
        }
    }

    if (key == GLFW_KEY_D) {
        glm::vec3 direction = getDirectionWithoutUp();
        glm::vec3 perp_direction = glm::cross(direction, up);
        if (action == GLFW_PRESS) {
            incrementVelocity(perp_direction);
            return;
        }
        else if (action == GLFW_RELEASE) {
            incrementVelocity(-perp_direction);
            return;
        }
    }

    if (key == GLFW_KEY_UP) {
        viewDirection = glm::rotate(viewDirection, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    if (key == GLFW_KEY_DOWN) {
        viewDirection = glm::rotate(viewDirection, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    if (key == GLFW_KEY_LEFT) {
        viewDirection = glm::rotate(viewDirection, glm::radians(-1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    if (key == GLFW_KEY_RIGHT) {
        viewDirection = glm::rotate(viewDirection, glm::radians(-1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    }
}
