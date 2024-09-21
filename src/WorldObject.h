#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>         // For glm::value_ptr

#include <string>
#include <memory>

#include "shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Transformer.h"
#include "VertexArray.h"

class WorldObject {
public:
    WorldObject(const std::string& objFilePath, std::shared_ptr<Shader> shaderProgram);

    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);
    void setScale(const glm::vec3& scale);
    glm::mat4 getModelMatrix() const;
    void Bind() const;
    void Draw() const;

    std::shared_ptr<Shader> shader;

private:
    std::unique_ptr<VertexBuffer> vertexBuffer = nullptr;
    std::unique_ptr<IndexBuffer> indexBuffer = nullptr;
    VertexArray vertexArray;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    mutable glm::mat4 modelMatrix;

    void updateModelMatrix() const;
};