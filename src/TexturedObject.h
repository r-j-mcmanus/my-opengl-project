#pragma once

#include <glm/glm.hpp>
#include "Errors.h"
#include "memory"
#include "shader.h"

float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

class TexturedObject {
public:
    TexturedObject(unsigned int textureID): m_TextureID(textureID){
        GLCall(glGenVertexArrays(1, &VAO));
        GLCall(glGenBuffers(1, &VBO));
        GLCall(glGenBuffers(1, &EBO));

        GLCall(glBindVertexArray(VAO));

        GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

        // position attribute
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
        GLCall(glEnableVertexAttribArray(0));
        // color attribute
        GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
        GLCall(glEnableVertexAttribArray(1));
        // texture coord attribute
        GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
        GLCall(glEnableVertexAttribArray(2));
    }

    void set_shader(std::shared_ptr<Shader> shader) {
        m_shader = shader;
    }

    void draw() const {
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
        m_shader->Bind();
        GLCall(glBindVertexArray(VAO));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    }

private:
    unsigned int m_TextureID = 0;
    unsigned int VBO = 0;
    unsigned int VAO = 0;
    unsigned int EBO = 0;
    std::shared_ptr<Shader> m_shader = NULL;

};