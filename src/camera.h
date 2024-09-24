#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>         // For glm::value_ptr

#include <iostream>

#include "Transformer.h"
#include "EventListenerInterface.h"

class Camera: public Transformer, public EventListenerInterface {
public:
    Camera(const glm::vec3& position, const glm::vec3& viewDirection, const glm::vec3& up, float fov, float aspectRatio, float nearPlane, float farPlane);

    void setAspectRatio(const int width, const int height);
    
    void setPosition(const glm::vec3& newPosition);

    void setTarget(const glm::vec3& target);

    void setAspectRatio(float newAspectRatio);

    glm::mat4 getViewMatrix() const;

    glm::mat4 getDirectionMatrix() const;

    glm::mat4 getProjectionMatrix() const;

    void update(float dt);

private:
    glm::vec3 viewDirection;
    glm::vec3 up;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    void updateViewMatrix();

    void updateViewMatrix(const glm::vec3& target);

    void updateProjectionMatrix();

    glm::vec3 getDirectionWithoutUp() const;

    void onKeyEvent(int key, int scancode, int action, int mods) override;
};
