
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>

#include "OBJParser.h"

bool OBJParser::parse(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;
        if (prefix == "v") {
            parseVertex(line);
        }
        else if (prefix == "vt") {
            parseTextureCoord(line);
        }
        else if (prefix == "vn") {
            parseNormal(line);
        }
        else if (prefix == "f") {
            parseFace(line);
        }
    }

    file.close();
    return true;
}

void OBJParser::parseVertex(const std::string& line) {
    Vertex v;
    std::istringstream ss(line.substr(2));
    ss >> v.x >> v.y >> v.z;
    vertices.push_back(v);
}

void OBJParser::parseTextureCoord(const std::string& line) {
    TextureCoord t;
    std::istringstream ss(line.substr(3));
    ss >> t.u >> t.v;
    textureCoords.push_back(t);
}

void OBJParser::parseNormal(const std::string& line) {
    Normal n;
    std::istringstream ss(line.substr(3));
    ss >> n.nx >> n.ny >> n.nz;
    normals.push_back(n);
}

void OBJParser::parseFace(const std::string& line) {
    // obj face lines can be of the form
    // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
    // where vi is the vertex index
    // vti is the texture index
    // vni is the normal vector index
    Face f;
    std::istringstream ss(line.substr(2));
    std::string token;
    while (ss >> token) {
        std::istringstream tokenStream(token);
        std::string index;
        std::vector<std::string> indices;
        while (std::getline(tokenStream, index, '/')) {
            indices.push_back(index);
        }
        if (!indices.empty()) {
            f.vertexIndices.push_back(std::atoi(indices[0].c_str()) - 1);
            if (indices.size() > 1 && !indices[1].empty()) {
                f.textureIndices.push_back(std::atoi(indices[1].c_str()) - 1);
            }
            if (indices.size() > 2 && !indices[2].empty()) {
                f.normalIndices.push_back(std::atoi(indices[2].c_str()) - 1);
            }
        }
    }
    faces.push_back(f);
}
