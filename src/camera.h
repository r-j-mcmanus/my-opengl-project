#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>         // For glm::value_ptr

#include "Transformer.h"

class Camera: public Transformer {
public:
    Camera(const glm::vec3& position, const glm::vec3& viewDirection, const glm::vec3& up, float fov, float aspectRatio, float nearPlane, float farPlane)
        : Transformer(position), viewDirection(viewDirection), up(up), fov(fov), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane)
    {
        updateViewMatrix();
        updateProjectionMatrix();
    }

    void setAspectRatio(const int width, const int height) {
        aspectRatio = (float)width / (float)height;
        updateProjectionMatrix();
    }

    void setPosition(const glm::vec3& newPosition) {
        Transformer::setPosition(newPosition);
        updateViewMatrix();
    }

    void setTarget(const glm::vec3& target) {
        viewDirection = glm::normalize(Transformer::getPosition() - target);
        updateViewMatrix(target);
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

    void update(float dt) {
        Transformer::update(dt);
        updateViewMatrix();
    }

private:
    glm::vec3 viewDirection;
    glm::vec3 up;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    void updateViewMatrix() {
        glm::vec3 position = Transformer::getPosition();
        glm::vec3 target = position - viewDirection;

        viewMatrix = glm::lookAt(Transformer::getPosition(), target, up);
    }

    void updateViewMatrix(const glm::vec3& target) {
        viewMatrix = glm::lookAt(Transformer::getPosition(), target, up);
    }

    void updateProjectionMatrix() {
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }
};
