
#include "WorldObject.h"
#include <string>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "OBJParser.h"



// Implementation
WorldObject::WorldObject(const std::string& objFilePath, std::shared_ptr<Shader> shaderProgram)
    : shader(shaderProgram), position(0.0f), rotation(0.0f), scale(1.0f), modelMatrix(1.0f) {
    OBJParser obj;
    obj.parse(objFilePath);

    // copy our vertices array in a buffer for OpenGL to use
    vertexBuffer = std::make_unique<VertexBuffer>(obj);

    int size = 3; // Specifies the number of components per generic vertex attribute.
    int stride = size * sizeof(float); // Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
    unsigned int index = 0; // This is the index of the vertex attribute. (??)
    const void* pointer = 0; // pointer (which is actually an offset) to where in the buffer data the attribute begins (used for interlaced data)
    // index, size, type, normalized, stride, pointer
    vertexArray.BindVertexBuffer(*vertexBuffer, index, size, GL_FLOAT, GL_TRUE, stride, pointer);
    
    indexBuffer = std::make_unique <IndexBuffer>(obj);
    vertexArray.BindIndexBuffer(*indexBuffer);
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
    shader->Bind();
}


void WorldObject::Draw() const{
    vertexArray.Draw();
}