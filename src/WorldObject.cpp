
#include "WorldObject.h"
#include <string>
#include "OBJParser.h"



// Implementation
WorldObject::WorldObject(const std::string& objFilePath, std::shared_ptr<Shader> shaderProgram)
    : shader(shaderProgram), position(0.0f), rotation(0.0f), scale(1.0f), modelMatrix(1.0f) {
    OBJParser obj;
    obj.parse(objFilePath);
    vertexCount = obj.getVertexCount();

    GLCall(glGenVertexArrays(1, &VAO));

    // vbo
    GLCall(glGenBuffers(1, &VBO));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));

    GLCall(glBufferData(GL_ARRAY_BUFFER, 
        obj.getBufferDataSize(),
        obj.getBufferDataPtr(),
        GL_STATIC_DRAW));

    // bao
    GLCall(glBindVertexArray(VAO));

    // position attribute
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0));
    // normal attribute, note offset at the end
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(1));

    GLCall(glBindVertexArray(0));
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

void WorldObject::draw() const {
    GLCall(glBindVertexArray(VAO));

    GLCall(glDrawArrays(GL_TRIANGLES, 0, vertexCount));

    GLCall(glBindVertexArray(0));
}