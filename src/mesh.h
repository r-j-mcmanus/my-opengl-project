#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>

#include "shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

enum TextureType {
    diffuse,
    specular
};

struct Texture {
    unsigned int id;
    TextureType type;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader& shader);
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};