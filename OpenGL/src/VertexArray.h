#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	VertexArray(const VertexArray& C);
	
	void AddBuffer(const VertexBuffer& vb,const VertexBufferLayout& layout);

	void DeleteVertexArray() const;

	void Bind() const;
	void Unbind() const;
};
