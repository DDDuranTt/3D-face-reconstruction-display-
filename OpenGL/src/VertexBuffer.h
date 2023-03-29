#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const VertexBuffer& C);

	void DeleteVertexBuffer() const;

	void Bind() const;
	void Unbind() const;

};