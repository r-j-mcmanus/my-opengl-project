#include "VertexBuffer.h"
#include "Errors.h"
#include "OBJParser.h"
#include <iostream>


void printVertices(const std::vector<Vertex>& vertices) {
    for (const auto& vertex : vertices) {
        std::cout << "Vertex: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
    }
}


VertexBuffer::VertexBuffer(const OBJParser& objParser)
{
    //m_Count = objParser.getVertexIndexArrayLength();
    //const unsigned int* vertexIndicesPtr = objParser.getVertexIndexArrayPtr();
    //unsigned int size = objParser.getVertexIndexArraySize();
    //std::cout << "vertexIndices length " << m_Count << " total vertexIndices size " << size << " size of VertexIndices " << sizeof(VertexIndices) << std::endl;

    //GLCall(glGenBuffers(1, &m_RendererId));
    //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
    //GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, vertexIndicesPtr, GL_STATIC_DRAW));

    const std::vector<Vertex> vertices = objParser.getVertices();

    //make a buffer and bind it for current use
    GLCall(glGenBuffers(1, &m_RendererId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
    //do not need to give data at initalisation, can be done latter neear use
    //GL_STATIC_DRAW as we will not be updating the triangle, but it is drawn a lot
    GLCall(glBufferData(GL_ARRAY_BUFFER, 
                        vertices.size() * sizeof(Vertex),
                        vertices.data(),
                        GL_STATIC_DRAW));

}

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    //make a buffer and bind it for current use
    GLCall(glGenBuffers(1, &m_RendererId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
    //do not need to give data at initalisation, can be done latter neear use
    //GL_STATIC_DRAW as we will not be updating the triangle, but it is drawn a lot
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererId))
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}