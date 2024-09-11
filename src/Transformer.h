#pragma once

#include <glm/vec3.hpp>

#include "helper.h"

class Transformer {
public:
    Transformer(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceliration) : position(position), velocity(velocity), acceliration(acceliration) {};
    Transformer(glm::vec3 position, glm::vec3 velocity) : position(position), velocity(velocity), acceliration(glm::vec3(0)) {};
    Transformer(glm::vec3 position) : position(position), velocity(glm::vec3(0)), acceliration(glm::vec3(0)) {};

    void update(float dt) {
        // Euler method
        velocity += dt * acceliration;
        position += dt * velocity;
    }

    void setVelocity(glm::vec3 velocity) { this->velocity = velocity; }
    void setAcceliration(glm::vec3 velocity) { this->acceliration = acceliration; }
    void setPosition(glm::vec3 position) { this->position = position; }

    void incrementVelocity(glm::vec3 velocity) { this->velocity += velocity; }
    void incrementAcceliration(glm::vec3 velocity) { this->acceliration += acceliration; }
    void incrementPosition(glm::vec3 position) { this->position += position; }

    glm::vec3 getPosition() const { return position; }

private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceliration;
};