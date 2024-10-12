
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>

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

    // at this point we want to put all the info about a vertex into a single vector
    // they are currently across multiple vectors and maps
    makeBufferVertex();

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
    // obj face lines are of the form
    // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
    // where vi is the vertex index
    // vti is the texture index
    // vni is the normal vector index

    std::istringstream ss(line.substr(2)); // skip 'f ' at the line start
    std::string vertexInfo; // will contain vi/vti/vni
    std::string vertexInfoElement; // will contain vi, vti or vni

    // stream the line into vertexInfo which contains v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
    // -1 as .obj files are 1 indexed
    
    Face face;
    for (int i = 0; i < 3; i++) {
        ss >> vertexInfo;
        std::istringstream firstVertex(vertexInfo);

        // stream vertexInfo between '/' into vertexInfoElement
        if (std::getline(firstVertex, vertexInfoElement, '/')) {
            face.v[i] = !vertexInfoElement.empty() ? std::stoi(vertexInfoElement) - 1 : -1;
        }
        if (std::getline(firstVertex, vertexInfoElement, '/')) {
            face.t[i] = !vertexInfoElement.empty() ? std::stoi(vertexInfoElement) - 1 : -1;
        }
        if (std::getline(firstVertex, vertexInfoElement, '/')) {
            face.n[i] = !vertexInfoElement.empty() ? std::stoi(vertexInfoElement) - 1 : -1;
        }
    }

    faces.push_back(face);
}

void OBJParser::makeBufferVertex() {
    for (const auto& face : faces) {
        for (int i = 0; i < 3; i++) {
            // ge the needed vertex and normal data
            Vertex v = vertices[face.v[i]];
            Normal n = normals[face.n[i]];

            bufferData.push_back(v.x);
            bufferData.push_back(v.y);
            bufferData.push_back(v.z);

            bufferData.push_back(n.nx);
            bufferData.push_back(n.ny);
            bufferData.push_back(n.nz);
        }
    }
}