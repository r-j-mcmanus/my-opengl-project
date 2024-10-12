#pragma once

#include <vector>
#include <map>
#include <string>

//https://en.wikipedia.org/wiki/Wavefront_.obj_file

struct Vertex {
    float x, y, z;
};

struct TextureCoord {
    float u, v;
};

struct Normal {
    float nx, ny, nz;
};

struct Face {
    unsigned int v[3], t[3], n[3];
};


class OBJParser {
public:
    bool parse(const std::string& filename);
    const std::vector<Vertex>& getVertices() const { return vertices; }
    const int getVerticesSize() const { return vertices.size() * sizeof(Vertex); }
    const Vertex* getVerticesPtr() const { return vertices.data(); }
    const std::vector<TextureCoord>& getTextureCoords() const { return textureCoords; }
    const std::vector<Normal>& getNormals() const { return normals; }

    const int getVertexCount() const { return faces.size() * 3; }
    const int getBufferDataSize() const { return bufferData.size() * sizeof(float); }
    float* getBufferDataPtr() { return bufferData.data(); }



private:
    unsigned int VertexCount = 0;

    std::vector<Vertex> vertices;
    std::vector<TextureCoord> textureCoords;
    std::vector<Normal> normals;
    std::vector<Face> faces;
    std::vector<float> bufferData;

    void parseVertex(const std::string& line);
    void parseTextureCoord(const std::string& line);
    void parseNormal(const std::string& line);
    void parseFace(const std::string& line);

    void makeBufferVertex();
};