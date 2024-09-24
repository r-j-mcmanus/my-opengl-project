#pragma once
#include <string>
#include <unordered_map>

class TextureManager {
public:
	void loadTexture(const std::string& textureKey, const std::string& objFilePath);
	void removeTexture(const std::string& textureKey);
	unsigned int getTextureId(const std::string& textureKey) const;
	~TextureManager();

private:
	std::unordered_map<std::string, unsigned int> TextureIDs;
};