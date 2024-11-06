#include <fstream>
#include <iostream>

#include <string>
#include <memory>

#include "GltfParser.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "Errors.h"
#include "nlohmann/json.hpp"

const int GLTF_FLAOT_COMPONENT_TYPE = 5126;

using json = nlohmann::json;

const std::map<std::string, int> mtypeMap{
    { "VEC3", 3 },
    { "VEC2", 2 },
    { "SCALAR", 1 }
};

std::vector<IndexData> GltfParser::parse(const std::string& filename) {
    /*
        reads the gltf file such that the buffers are loaded into vertex buffer objects, the vertex 
        attributa data is bound and returned is a vector of IndexData, describing which vbo and the
        byte offset and the size of the index array. 
    */

	std::ifstream f(filename);
	data = json::parse(f);

    parseBuffers();

    for (auto mesh : data["meshes"]) {
        std::string name = mesh["name"];
        for (auto primative : mesh["primitives"]) {
            // the accessor index for each of these properties of the mess
            // todo what if one of these is missing?
            int position = primative["attributes"]["POSITION"];
            int normal = primative["attributes"]["NORMAL"];
            int texCoord_0 = primative["attributes"]["TEXCOORD_0"];
            int indices = primative["indices"];
            int material = primative["material"];

            setAttribute(0, position);
            setAttribute(1, normal);
            setAttribute(3, texCoord_0);
            setIndices(indices);
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind buffer

    return vecIndexData;
}

const VertexAttribData GltfParser::getVertexAttribData(int accessorIndex) {
    /*
        combines data from the accessor corrisponding to the index, and the associated buffer view
    */
    VertexAttribData vertexAttribData;
    
    // Ensure accessorIndex is valid
    if (accessorIndex < 0 || accessorIndex >= data["accessors"].size()) {
        throw std::out_of_range("Invalid accessorIndex provided.");
    }
    const auto accessor = data["accessors"][accessorIndex];
    vertexAttribData.componentsPerVertex = mtypeMap.at(accessor["type"].get<std::string>());
    // the int given in the json for the componentType is already the enum for the appropriet GL definition
    vertexAttribData.dataType = accessor["componentType"].get<int>();
    const int bufferViewIndex = accessor["bufferView"];
    vertexAttribData.normalized = accessor.contains("normalized") ? accessor["normalized"].get<bool>() : GL_FALSE;
    vertexAttribData.count = accessor["count"].get<int>();

    // grab data from the buffer view
    if (bufferViewIndex < 0 || bufferViewIndex >= data["bufferViews"].size()) {
        throw std::out_of_range("Invalid bufferView index in accessor.");
    }
    const auto bufferView = data["bufferViews"][bufferViewIndex];
    vertexAttribData.bufferIndex = bufferView["buffer"].get<int>();
    vertexAttribData.byteOffset = bufferView["byteOffset"].get<int>();
    // byteStride = 0 when tightly packed
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
    if (vertexAttribData.dataType == GLTF_FLAOT_COMPONENT_TYPE) {
        glVertexAttribPointer(
            attribArray,
            vertexAttribData.componentsPerVertex,
            vertexAttribData.dataType,
            vertexAttribData.normalized ? GL_TRUE : GL_FALSE,
            vertexAttribData.byteStride,
            (void*)vertexAttribData.byteOffset
        );
    }
    else {
        // pass int types to the glVertexAttrib I Pointer, prevents casting to flaots
        glVertexAttribIPointer(
            attribArray,
            vertexAttribData.componentsPerVertex,
            vertexAttribData.dataType,
            vertexAttribData.byteStride,
            reinterpret_cast<void*>(vertexAttribData.byteOffset)
        );
    }
}

void GltfParser::setIndices(int indicesAccessorIndex) {
    /*
        Sets the index for the mesh given the accessor index for the indices
    */
    VertexAttribData vertexAttribData = getVertexAttribData(indicesAccessorIndex);


    const unsigned int VBO = VBOs[vertexAttribData.bufferIndex];
    IndexData indexData = { VBO, vertexAttribData.count, vertexAttribData.dataType, vertexAttribData.byteOffset };
    vecIndexData.push_back(indexData);

    //// to bind the index part of the buffer as an ELEMENT_ARRAY_BUFFER
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);

    //glDrawElements(
    //    GL_TRIANGLES, 
    //    vertexAttribData.count,
    //    vertexAttribData.dataType,
    //    reinterpret_cast<void*>(vertexAttribData.byteOffset)
    //);

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
        GLCall(glGenBuffers(1, &VBO)); // make 1 array
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO)); // useing this array
        GLCall(glBufferData(GL_ARRAY_BUFFER, buffer_size, newCharArray.get(), GL_STATIC_DRAW)); // pass the read buffer to this vbo
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));  // Unbind to prevent accidental overwrite

        VBOs.push_back(VBO);
    }
    return true;
 }
