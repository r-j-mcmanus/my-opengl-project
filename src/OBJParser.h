#pragma once

#include <vector>
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

// obj face lines can be of the form
// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
// where vi is the vertex index
// vti is the texture index
// vni is the normal vector index
struct IndicesTriplet {
    unsigned int i, j, k;
};
struct VertexIndices {
    unsigned int i, j, k;
};
struct TextureIndices {
    unsigned int i, j, k;
};
struct NormalIndices {
    unsigned int i, j, k;
};


class OBJParser {
public:
    bool parse(const std::string& filename);
    const std::vector<Vertex>& getVertices() const { return vertices; }
    const int getVerticesSize() const { return vertices.size() * sizeof(Vertex); }
    const Vertex* getVerticesPtr() const { return vertices.data(); }
    const std::vector<TextureCoord>& getTextureCoords() const { return textureCoords; }
    const std::vector<Normal>& getNormals() const { return normals; }
    const std::vector<IndicesTriplet>& getVertexIndices() const { return vertexIndices; }

private:
    std::vector<Vertex> vertices;
    std::vector<TextureCoord> textureCoords;
    std::vector<Normal> normals;
    std::vector<IndicesTriplet> vertexIndices;
    std::vector<IndicesTriplet> textureIndices;
    std::vector<IndicesTriplet> normalIndices;

    void parseVertex(const std::string& line);
    void parseTextureCoord(const std::string& line);
    void parseNormal(const std::string& line);
    void parseFace(const std::string& line);
};