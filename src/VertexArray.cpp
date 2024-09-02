#include "VertexArray.h"
#include "renderer.h"
#include <gl/glew.h>

VertexArray::VertexArray() {
    // make the vertex array object that will link the array buffer
    // we make the vao with the id we have then bind the newly made vao
    // glGenVertexArrays sets the value of m_RendererID
    GLCall(glGenVertexArrays(1, &m_VertexArrayObjectID));
    GLCall(glBindVertexArray(m_VertexArrayObjectID));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &m_VertexArrayObjectID));
}

void VertexArray::Bind() const {
    GLCall(glBindVertexArray(m_VertexArrayObjectID));
}

void VertexArray::Unbind() const {
    GLCall(glBindVertexArray(0));
}

void VertexArray::AddVertexBuffer(const VertexBuffer& vb, unsigned int index, int size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) const{
    /*
    index = 0 as first atribute is what we want to use, is the index used in the vertex shader
    size = 2 as we have 2 floats in the position we want to use
    type = GL_FLOAT
    normalize = false as we are flaots 0<x<1
    stride = 2 as each vertex only has two data points (x pos and y pos, could also have texture / normals ect n the sasme vertex)
        offset in the vertex to the data we want (sat we wanted normal, use (const void*) 8, for example)

    */
    
    Bind();
    vb.Bind();
    GLCall(glEnableVertexAttribArray(index)); // this line is what binds the buffer to the vao
    // tell open gl that we are allowed to draw this buffer
    // this allows us to ask to draw the vertexbuffer and index buffer again without spesifying it again
    GLCall(glVertexAttribPointer(index, size, type, normalized, stride, pointer));
}

void VertexArray::SetIndexBuffer(const IndexBuffer& ib) {
    Bind();
    ib.Bind(); // bind the index buffer to say which elelemnts in the vertex array we want to draw
    m_IndexCount = ib.GetCount();  // Store the index count for use in Draw
}

void VertexArray::Draw() const {
    // draw ibo as it prevents duplicating vertex info in the gpu memory
    // what are we drawing, what index do we start, how many verts ect
    // this will draw the currently bound buffer
    // has to be unsigned
    // null ptr as we have bound ibo to GL_ELEMENT_ARRAY_BUFFER
    Bind();
    GLCall(glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr));
}
