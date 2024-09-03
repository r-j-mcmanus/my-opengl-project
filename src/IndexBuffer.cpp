#include "IndexBuffer.h"
#include "errors.h"
#include <iostream>


void printVertices(const std::vector<VertexIndices>& vertices) {
    for (const auto& vertex : vertices) {
        std::cout << "Index: (" << vertex.i << ", " << vertex.j << ", " << vertex.k << ")" << std::endl;
    }
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    GLCall(glGenBuffers(1, &m_RendererId));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer(const OBJParser& objParser)
{
    const std::vector<VertexIndices> vertexIndices = objParser.getVertexIndices();
    m_Count = vertexIndices.size();

    GLCall(glGenBuffers(1, &m_RendererId));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        vertexIndices.size() * sizeof(VertexIndices),
                        vertexIndices.data(),
                        GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererId))
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
