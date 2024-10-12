// see https://learnopengl.com/Advanced-OpenGL/Cubemaps

#include <vector>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include "Skybox.h"
#include "shader.h"
#include "OBJParser.h"

#include <iostream>


#include <glm/gtc/matrix_transform.hpp>


const float skyboxVertices[] =
{
    //   Coordinates
    -1.0f, -1.0f,  1.0f,//        7--------6
     1.0f, -1.0f,  1.0f,//       /|       /|
     1.0f, -1.0f, -1.0f,//      4--------5 |
    -1.0f, -1.0f, -1.0f,//      | |      | |
    -1.0f,  1.0f,  1.0f,//      | 3------|-2
     1.0f,  1.0f,  1.0f,//      |/       |/
     1.0f,  1.0f, -1.0f,//      0--------1
    -1.0f,  1.0f, -1.0f
};

const unsigned int skyboxIndices[] =
{
    // Right
    1, 2, 6,
    6, 5, 1,
    // Left
    0, 4, 7,
    7, 3, 0,
    // Top
    4, 5, 6,
    6, 7, 4,
    // Bottom
    0, 3, 2,
    2, 1, 0,
    // Back
    0, 1, 5,
    5, 4, 0,
    // Front
    3, 7, 6,
    6, 2, 3
};


void PrintMatrix(const glm::mat4& matrix) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            std::cout << matrix[col][row] << "\t";  // Access the element at [col][row]
        }
        std::cout << std::endl;  // Newline after each row
    }
}


Skybox::Skybox(std::shared_ptr<Shader> shader): shader(shader) {
    // copy our vertices array in a buffer for OpenGL to use
    GLCall(glGenVertexArrays(1, &skyboxVAO));
    GLCall(glGenBuffers(1, &skyboxVBO));
    GLCall(glGenBuffers(1, &skyboxEBO));
    GLCall(glGenTextures(1, &cubemapTextureID));

    GLCall(glBindVertexArray(skyboxVAO));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), skyboxIndices, GL_STATIC_DRAW));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0));
    //unbind all buffers
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    shader->Bind();
    shader->setUniformInt("skybox", 0);
}

// Load the cube map textures for the skybox
void Skybox::LoadSkybox(const std::vector<std::string>& faces) {
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID));

    int width, height, nrChannels;

    for (GLuint i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
 }

void Skybox::Render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    GLCall(glDepthFunc(GL_LEQUAL));  // Ensure the skybox is rendered in the far depth

    const glm::mat4 view = glm::mat4(glm::mat3(viewMatrix));  // Remove translation component

    //PrintMatrix(view);
    //PrintMatrix(projectionMatrix);

    shader->Bind();
    shader->setUniformMat4("view", view);
    shader->setUniformMat4("projection", projectionMatrix);

    GLCall(glBindVertexArray(skyboxVAO));
    GLCall(glActiveTexture(GL_TEXTURE0)); // which textrure we bind to, some shaders can minipulate multiple textures at once
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID));
    GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0));
    GLCall(glBindVertexArray(0));
    
    GLCall(glDepthFunc(GL_LESS));  // Reset depth function
}

Skybox::~Skybox() {
    GLCall(glDeleteTextures(1, &cubemapTextureID));
    GLCall(glDeleteVertexArrays(1, &skyboxVAO));
    GLCall(glDeleteBuffers(1, &skyboxVBO));
    GLCall(glDeleteBuffers(1, &skyboxEBO));
}
