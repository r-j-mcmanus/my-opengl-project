#pragma once
#include "nlohmann/json.hpp"
#include <vector>

using json = nlohmann::json;

struct VertexAttribData {
    int componentsPerVertex;
    int bufferIndex;
    int byteOffset;
    int dataType;  // the datatype index used in gltf is the same as that in glfw
    int byteStride;
    bool normalized;
    int count;
};

struct IndexData {
    const unsigned int VBO;
    const int count;
    const int dataType; // the datatype index used in gltf is the same as that in glfw
    int byteOffset;
};

class GltfParser {
public:
    std::vector<IndexData> parse(const std::string& filename);
private:
    bool parseBuffers();

    void setAttribute(int attribArray, int accessorIndex);
    void setIndices(int indicesAccessorIndex);

    const VertexAttribData getVertexAttribData(int temp);

    unsigned int EBO;
    std::vector<unsigned int> VBOs;
    std::vector<IndexData> vecIndexData;
    unsigned int currentVBO=0;

    json data;
};