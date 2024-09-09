#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>         // For glm::value_ptr


class Camera {
public:
    Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, float fov, float aspectRatio, float nearPlane, float farPlane)
        : position(position), target(target), up(up), fov(fov), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane)
    {
        updateViewMatrix();
        updateProjectionMatrix();
    }

    void setAspectRatio(const int width, const int height) {
        aspectRatio = (float)width / (float)height;
        updateProjectionMatrix();
    }

    void setPosition(const glm::vec3& newPosition) {
        position = newPosition;
        updateViewMatrix();
    }

    void setTarget(const glm::vec3& newTarget) {
        target = newTarget;
        updateViewMatrix();
    }

    void setAspectRatio(float newAspectRatio) {
        aspectRatio = newAspectRatio;
        updateProjectionMatrix();
    }

    glm::mat4 getViewMatrix() const {
        return viewMatrix;
    }

    glm::mat4 getProjectionMatrix() const {
        return projectionMatrix;
    }

    void translate(glm::vec3 translation) {
        position += translation;
        target += translation;
        updateViewMatrix();
    }

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    void updateViewMatrix() {
        viewMatrix = glm::lookAt(position, target, up);
    }

    void updateProjectionMatrix() {
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }
};
