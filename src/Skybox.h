#pragma once

#include <memory>

#include "shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

class Skybox {
private:
    GLuint skyboxVAO, skyboxVBO, skyboxEBO;
    GLuint cubemapTextureID;

    std::shared_ptr<Shader> shader;

public:
    Skybox(std::shared_ptr<Shader> shader);

    // Load the cube map textures for the skybox
    void LoadSkybox(const std::vector<std::string>& faces);

    // Render the skybox
    void Render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

    ~Skybox();
};
