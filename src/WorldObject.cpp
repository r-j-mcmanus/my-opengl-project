
#include "WorldObject.h"
#include <string>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "OBJParser.h"



// Implementation
WorldObject::WorldObject(const std::string& objFilePath, Shader& shaderProgram)
    : shader(shaderProgram), position(0.0f), rotation(0.0f), scale(1.0f), modelMatrix(1.0f) {
    OBJParser obj;
    obj.parse(objFilePath);

    vertexBuffer = VertexBuffer(obj);
    indexBuffer = IndexBuffer(obj);

    int vertexDim = 3;
    int stride = 3;
    vertexArray.BindVertexBuffer(vertexBuffer, 0, vertexDim, GL_FLOAT, GL_TRUE, sizeof(float) * stride, (const void*)0);
    vertexArray.BindIndexBuffer(indexBuffer);
}

void WorldObject::setPosition(const glm::vec3& position) {
    this->position = position;
    updateModelMatrix();
}

void WorldObject::setRotation(const glm::vec3& rotation) {
    this->rotation = rotation;
    updateModelMatrix();
}

void WorldObject::setScale(const glm::vec3& scale) {
    this->scale = scale;
    updateModelMatrix();
}

glm::mat4 WorldObject::getModelMatrix() const {
    return modelMatrix;
}

void WorldObject::updateModelMatrix() const {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationMatrix = (
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
    );
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void WorldObject::Bind() const{
    shader.Bind();
}


void WorldObject::Draw() const{
    vertexArray.Draw();
}