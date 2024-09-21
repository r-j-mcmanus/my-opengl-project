#include "VertexBuffer.h"
#include "Errors.h"
#include "OBJParser.h"
#include <iostream>


void printVertices(const std::vector<Vertex>& vertices) {
    for (const auto& vertex : vertices) {
        std::cout << "Vertex: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
    }
}

void printVerticesData(const std::vector<Vertex>& vertices) {
    const float* vertexPtr = reinterpret_cast<const float*>(vertices.data());
    int length = vertices.size() * 3;
    for (int i = 0; i < length; ++i) {
        std::cout << *(vertexPtr + i) << " ";  // Access the element using the pointer
    }
    std::cout << "\n\n";
}


VertexBuffer::VertexBuffer(const OBJParser& objParser)
{
    const std::vector<Vertex>& vertices = objParser.getVertices();

    //make a buffer and bind it for current use
    GLCall(glGenBuffers(1, &m_RenderVBO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderVBO));
    //do not need to give data at initalisation, can be done latter neear use
    //GL_STATIC_DRAW as we will not be updating the triangle, but it is drawn a lot
    GLCall(glBufferData(GL_ARRAY_BUFFER, 
                        objParser.getVerticesSize(),
                        objParser.getVerticesPtr(),
                        GL_STATIC_DRAW));
    Unbind();
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    //make a buffer and bind it for current use
    GLCall(glGenBuffers(1, &m_RenderVBO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderVBO));
    // We specify buffer data here; it could also be done later if needed.
    //GL_STATIC_DRAW as we will not be updating the triangle, but it is drawn a lot
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    Unbind();
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RenderVBO));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderVBO));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}