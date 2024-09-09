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
    GLCall(glGenBuffers(1, &m_RendererId)); // generates one new buffer object and assigns its unique identifier to m_RendererId
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId)); // binds the buffer GL_ELEMENT_ARRAY_BUFFER to the buffer with the id m_RendererId
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer(const OBJParser& objParser)
{
    const std::vector<VertexIndices>& vertexIndices = objParser.getVertexIndices();
    //printVertices(vertexIndices);

    this->m_Count = vertexIndices.size() * 3; // *3 as we are using 3d objects
    std::cout << "IndexBuffer count * sizeof(unsigned int) " << vertexIndices.size() << " " << sizeof(VertexIndices) << std::endl;

    GLCall(glGenBuffers(1, &m_RendererId)); //makes 1 buffer and bind it for use
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, // specifies that the buffer you're binding is going to be used as an element buffer, which stores indices that reference vertices in a vertex array.
                        m_RendererId));
    
    GLuint target = GL_ELEMENT_ARRAY_BUFFER;   // specifies that the buffer you're binding is going to be used as an element buffer, which stores indices that reference vertices in a vertex array
    GLsizeiptr size = vertexIndices.size() * sizeof(VertexIndices);  // the size in bytes of the buffer object's new data store.
    const void* data = vertexIndices.data();  // pointer to data that will be copied into the data store for initialization, or NULL if no data is to be copied.
    GLenum usage = GL_STATIC_DRAW;  // the expected usage pattern of the data store, helps OpenGL optimize the buffer for performance.
    GLCall(glBufferData(target, size, data, usage));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererId));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
