#include "VertexArray.h"
#include "Errors.h"
#include <gl/glew.h>

VertexArray::VertexArray() {
    // make the vertex array object that will link the array buffer
    // we make the vao with the id we have then bind the newly made vao
    // glGenVertexArrays sets the value of m_RendererID
    GLCall(glGenVertexArrays(1, &m_VertexArrayObjectID));
    GLCall(glBindVertexArray(m_VertexArrayObjectID));
}

VertexArray::~VertexArray() {
    GLCall(glBindVertexArray(0));
    GLCall(glDeleteVertexArrays(1, &m_VertexArrayObjectID));
}

void VertexArray::Bind() const {
    GLCall(glBindVertexArray(m_VertexArrayObjectID));
}

void VertexArray::Unbind() const {
    GLCall(glBindVertexArray(0));
}

void VertexArray::BindVertexBuffer(const VertexBuffer& vb, unsigned int index, int size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) const{
    /*
    unsigned int index - This is the index of the vertex attribute. (??)
    int size - Specifies the number of components per generic vertex attribute.
    int stride -  Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
    const void* pointer -  pointer (which is actually an offset) to where in the buffer data the attribute begins (used for interlaced data)
    */
    
    Bind();
    vb.Bind();
    // this line is what binds the buffer to the vao
    GLCall(glVertexAttribPointer(index, size, type, normalized, stride, pointer));
    // tell open gl that we are allowed to draw this buffer
    // this allows us to ask to draw the vertexbuffer and index buffer again without spesifying it again
    GLCall(glEnableVertexAttribArray(index)); // use the data in the vertex buffer for the specified index attribute when drawing
}

void VertexArray::BindIndexBuffer(const IndexBuffer& ib) {
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
