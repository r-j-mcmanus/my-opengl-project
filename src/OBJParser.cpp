
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
    IndicesTriplet v_index;
    IndicesTriplet t_index;
    IndicesTriplet n_index;

    std::istringstream ss(line.substr(2)); // skip f
    std::string vertexInfo;
    std::string vertexInfoElement;

    // stream the line into vertexInfo which contains v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
    // strean vertexInfo between '/' into relevant vectors

    ss >> vertexInfo;
    std::istringstream firstVertex(vertexInfo);
    if (std::getline(firstVertex, vertexInfoElement, '/')) { 
        v_index.i = !vertexInfoElement.empty() ? std::stoi(vertexInfoElement) - 1 : -1;
    }
    if (std::getline(firstVertex, vertexInfoElement, '/')) {
        t_index.i = !vertexInfoElement.empty() ? std::stoi(vertexInfoElement) - 1 : -1;
    }
    if (std::getline(firstVertex, vertexInfoElement, '/')) {
        n_index.i = !vertexInfoElement.empty() ? std::stoi(vertexInfoElement) - 1 : -1;
    }

    ss >> vertexInfo;
    firstVertex.str(vertexInfo);
    firstVertex.clear();
    if (std::getline(firstVertex, vertexInfoElement, '/')) {
        v_index.j = !vertexInfoElement.empty() ? std::stoi(vertexInfoElement) - 1 : -1;
    }
    if (std::getline(firstVertex, vertexInfoElement, '/')) {
        t_index.j = !vertexInfoElement.empty() ? std::stoi(vertexInfoElement) - 1 : -1;
    }
    if (std::getline(firstVertex, vertexInfoElement, '/')) {
        n_index.j = !vertexInfoElement.empty() ? std::stoi(vertexInfoElement) - 1 : -1;
    }

    ss >> vertexInfo;
    firstVertex.str(vertexInfo);
    firstVertex.clear();
    if (std::getline(firstVertex, vertexInfoElement, '/')) {
        v_index.k = !vertexInfoElement.empty() ? std::stoi(vertexInfoElement) - 1 : -1;
    }
    if (std::getline(firstVertex, vertexInfoElement, '/')) {
        t_index.k = !vertexInfoElement.empty() ? std::stoi(vertexInfoElement) - 1 : -1;
    }
    if (std::getline(firstVertex, vertexInfoElement, '/')) {
        n_index.k = !vertexInfoElement.empty() ? std::stoi(vertexInfoElement) - 1 : -1;
    }

    vertexIndices.push_back(v_index);
    textureIndices.push_back(t_index);
    normalIndices.push_back(n_index);
}