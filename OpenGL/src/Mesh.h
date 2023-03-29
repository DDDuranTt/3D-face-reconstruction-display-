#pragma once
#include <string>
#include <vector>

#include "glm\glm.hpp"

#include "VertexArray.h"
#include "VertexBuffer.h"

#include "IndexBuffer.h"
#include "Texture.h"

class VertexBufferLayout;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	float alpha;
};


class Mesh {
public:
	/*  网格数据  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	/*  函数  */
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	Mesh(const Mesh& C);
	~Mesh();
	void BindMesh()const;
	void UnbindMesh()const;

	void DeleteMesh();
private:
	/*  渲染数据  */
	VertexArray* VAO;
	VertexBuffer* VBO;
	IndexBuffer* EBO;
	VertexBufferLayout* VBL;
	/*  函数  */
	void setupMesh();
};
