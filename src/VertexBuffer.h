#pragma once

#include "OBJParser.h"

class VertexBuffer
{
private:
	unsigned int m_RendererId;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const OBJParser& objParser);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};