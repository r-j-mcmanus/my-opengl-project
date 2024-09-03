#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>         // For glm::value_ptr

#include "helper.h"

class Transformer {
public:
    Transformer() : modelMatrix(1.0f) {}
    Transformer(const glm::vec3& position, const glm::vec3& rotation, const float scaleX, const float scaleY, const float scaleZ){
        modelMatrix = glm::mat4(1.0f); 
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, scaleZ));

        glm::mat4 rotationMatrix = createRotationMatrix(rotation);
        modelMatrix = rotationMatrix * modelMatrix;

        modelMatrix = glm::translate(modelMatrix, position);
    }

    void translate(const glm::vec3& position) {
        modelMatrix = glm::translate(modelMatrix, position);
    }

    void Rotate(const glm::vec3& rotation) {
        glm::mat4 rotationMatrix = createRotationMatrix(rotation);
        modelMatrix = rotationMatrix * modelMatrix;
    }

    void Scale(const glm::vec3& scale) {
        modelMatrix = glm::scale(modelMatrix, scale);
    }

    glm::mat4 getModelMatrix() const {
        return modelMatrix;
    }

private:
    glm::mat4 modelMatrix;
};
