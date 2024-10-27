#include <fstream>
#include <iostream>

#include <string>
#include <memory>

#include "GltfParser.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "Errors.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

const std::map<std::string, int> mtypeMap{
    { "VEC3", 3 },
    { "VEC2", 2 },
    { "SCALAR", 1 }
};

static void parseCubeBin()
{
    struct vec3 {
        float x;
        float y;
        float z;
    };

    struct vec2 {
        float x;
        float y;
    };

    // Declare the arrays to hold the parsed data
    vec3 positionArray[24];
    vec3 normalArray[24];
    vec2 texArray[24];
    unsigned short faceArray[36];

    // Open the binary file in input mode
    std::ifstream file("res/gltf/cube.bin", std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    file.read(reinterpret_cast<char*>(positionArray), sizeof(positionArray));
    if (!file) {
        std::cerr << "Error reading positionArray data!" << std::endl;
        return;
    }

    file.read(reinterpret_cast<char*>(normalArray), sizeof(normalArray));
    if (!file) {
        std::cerr << "Error reading normalArray data!" << std::endl;
        return;
    }

    file.read(reinterpret_cast<char*>(texArray), sizeof(texArray));
    if (!file) {
        std::cerr << "Error reading texArray data!" << std::endl;
        return;
    }

    file.read(reinterpret_cast<char*>(faceArray), sizeof(faceArray));
    if (!file) {
        std::cerr << "Error reading faceArray data!" << std::endl;
        return;
    }

    // Close the file
    file.close();

    // Display the parsed data (optional)
    std::cout << "positionArray:" << std::endl;
    for (int i = 0; i < 24; ++i) {
        std::cout << positionArray[i].x << " " << positionArray[i].y << " " << positionArray[i].z << std::endl;
    }
    std::cout << std::endl;

    std::cout << "normalArray:" << std::endl;
    for (int i = 0; i < 24; ++i) {
        std::cout << normalArray[i].x << " " << normalArray[i].y << " " << normalArray[i].z << std::endl;
    }
    std::cout << std::endl;

    std::cout << "texArray:" << std::endl;
    for (int i = 0; i < 24; ++i) {
        std::cout << texArray[i].x << " " << texArray[i].y << std::endl;
    }
    std::cout << std::endl;

    std::cout << "faceArray:" << std::endl;
    for (int i = 0; i < 12; ++i) {
        std::cout << faceArray[i] << " " << faceArray[i + 1] << " " << faceArray[i + 2] << std::endl;
    }
    std::cout << std::endl;
}

bool GltfParser::parse(const std::string& filename) {

    parseCubeBin();

	std::ifstream f(filename);
	data = json::parse(f);

    parseBuffers();

    for (auto mesh : data["meshes"]) {
        int position = mesh["primitives"][0]["primitives"]["POSITION"];
        int normal = mesh["primitives"][0]["primitives"]["POSITION"];
        int texCord_0 = mesh["primitives"][0]["primitives"]["POSITION"];
        int indices = mesh["primitives"][0]["primitives"]["POSITION"];

        setAttribute(0, position);
        setAttribute(1, normal);
        setAttribute(3, texCord_0);
        setIndices(indices);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind buffer

    return true;
}

const VertexAttribData GltfParser::getVertexAttribData(int accessorIndex) {
    VertexAttribData vertexAttribData;
    
    const auto accessor = data["accessors"][accessorIndex];
    vertexAttribData.componentsPerVertex = mtypeMap.at(accessor["type"].get<std::string>());
    // the int given in the json for the componentType is already the enum for the appropriet GL definition
    vertexAttribData.dataType = accessor["componentType"].get<int>();
    const int bufferViewIndex = accessor["bufferView"];
    vertexAttribData.normalized = accessor.contains("normalized") ? accessor["normalized"].get<bool>() : GL_FALSE;

    const auto bufferView = data["bufferViews"][bufferViewIndex];
    vertexAttribData.bufferIndex = bufferView["buffer"].get<int>();
    vertexAttribData.byteOffset = bufferView["byteOffset"].get<int>();
    // 0 when tightly packed
    vertexAttribData.byteStride = bufferView.contains("byteStride") ? bufferView["byteStride"].get<int>() : 0;

    return vertexAttribData;
}

void GltfParser::setAttribute(int attribArray, int accessorIndex) {
    /*
        Sets the position attribute for the mesh given the accessor index for the position
    */
    VertexAttribData vertexAttribData = getVertexAttribData(accessorIndex);

    const unsigned int VBO = VBOs[vertexAttribData.bufferIndex];

    if (currentVBO != VBO) {
        currentVBO = VBO;
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }
        
    glEnableVertexAttribArray(attribArray);
    glVertexAttribPointer(
        attribArray,
        vertexAttribData.componentsPerVertex,
        vertexAttribData.dataType,
        vertexAttribData.normalized ? GL_TRUE : GL_FALSE,
        vertexAttribData.byteStride,    
        (void*)vertexAttribData.byteOffset
    );
}

void GltfParser::setIndices(int indicesAccessorIndex) {

    // the buffer the index is stored in
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, // buffer : data["bufferViews"][viewIndex]["target"]
        72, // size : 768 = data["bufferViews"][viewIndex]["byteLength"]
        &data[768], // *data : 768 = data["bufferViews"][viewIndex]["byteOffset"]
        GL_STATIC_DRAW // usage
    );


}

bool GltfParser::parseBuffers(){
    /*
    Here we load the buffers into memory so that we can pass them to opengl as part of the processing of the bufferViews
    */
    for (auto& buffer : data["buffers"]) {
        int buffer_size = buffer["byteLength"].get<int>();
        auto newCharArray = std::make_unique<char[]>(buffer_size);

        std::string uri = buffer["uri"].get<std::string>();
        std::ifstream file("res/gltf/" + uri, std::ios::binary);
        if (!file) {
            std::cerr << "Failed to open file: " << uri << std::endl;
            return false;
        }

        file.read(newCharArray.get(), buffer_size);
        file.close();
        if (!file) {
            std::cerr << "Failed to read " << buffer_size << " bytes from file: " << uri << std::endl;
            return false;
        }

        GLuint VBO;
        glGenBuffers(1, &VBO); // make 1 array
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // useing this array
        glBufferData(GL_ARRAY_BUFFER, buffer_size, newCharArray.get(), GL_STATIC_DRAW); // pass the read buffer to this vbo
        glBindBuffer(GL_ARRAY_BUFFER, 0);  // Unbind to prevent accidental overwrite

        VBOs.push_back(VBO);
    }
    return true;
 }
