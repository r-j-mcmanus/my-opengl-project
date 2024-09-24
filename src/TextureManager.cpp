
#include <string>
#include <unordered_map>

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "stb_image.h"

#include "TextureManager.h"

void TextureManager::loadTexture(const std::string& textureKey, const std::string& objFilePath) {

    // bind the id for the texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(objFilePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        TextureIDs[textureKey] = texture; // save the texture
    }
    else
    {
        std::cout << "Failed to load texture " << textureKey << std::endl;
    }
    stbi_image_free(data);
}

unsigned int TextureManager::getTextureId(const std::string& textureKey) const {
    auto it = TextureIDs.find(textureKey);
    if (it != TextureIDs.end()) {
        return it->second;
    }
    return 0;
}

void TextureManager::removeTexture(const std::string& textureKey) {
    auto it = TextureIDs.find(textureKey);
    if (it != TextureIDs.end()) {
        glDeleteTextures(1, &it->second);
        TextureIDs.erase(it);
    }
    else {
        std::cout << "Texture not found: " << textureKey << std::endl;
    }
}

TextureManager::~TextureManager() {
    for (const auto& texturePair : TextureIDs) {
        glDeleteTextures(1, &texturePair.second);
    }
    TextureIDs.clear();
}