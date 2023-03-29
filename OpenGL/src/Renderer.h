#pragma once
#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Model.h"


#pragma region Error Handling
#define ASSERT(x) if(!(x))__debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
#pragma endregion


class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va,const IndexBuffer& ib, Shader& shader) const;
	void Draw(const Model& model, Shader& shader) const;
	void Draw(const Model& model, Shader& shader, int u_texture)const;
};