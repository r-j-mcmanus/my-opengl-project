#pragma once
#include "nlohmann/json.hpp"
#include <vector>

using json = nlohmann::json;

struct VertexAttribData {
    int componentsPerVertex;
    int bufferIndex;
    int byteOffset;
    int dataType;
    int byteStride;
    bool normalized;
};

class GltfParser {
public:
    bool parse(const std::string& filename);
private:
    bool parseBuffers();

    void setAttribute(int attribArray, int accessorIndex);
    void setIndices(int indicesAccessorIndex);

    const VertexAttribData getVertexAttribData(int temp);

    unsigned int EBO;
    std::vector<unsigned int> VBOs;
    unsigned int currentVBO=0;

    json data;
};