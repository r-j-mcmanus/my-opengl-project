#pragma once


#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <gl/glew.h>

class VertexArray {
    /* this class manages the creation, binding and cleanup of a vertex object array, and its associated VertexBuffer and IndexBuffer*/
private:
    unsigned int m_VertexArrayObjectID;
    unsigned int m_IndexCount;

public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddVertexBuffer(const VertexBuffer& vb, unsigned int index, int size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) const;
    void SetIndexBuffer(const IndexBuffer& ib);

    void Draw() const;
};