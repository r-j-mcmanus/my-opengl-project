#pragma once

#include "OBJParser.h"

class IndexBuffer
{
private:
	unsigned int m_RendererId;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer(const OBJParser& objParser);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; };
};