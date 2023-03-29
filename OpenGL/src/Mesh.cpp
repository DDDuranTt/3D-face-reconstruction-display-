#include "Mesh.h"
#include "VertexBufferLayout.h"

#include <iostream>
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	for (int i = 0; i < this->vertices.size(); ++i) {
		this->vertices[i].Normal = glm::normalize(this->vertices[i].Normal);
	}
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

Mesh::Mesh(const Mesh& C)
{
	this->vertices = C.vertices;
	this->indices =  C.indices;
	this->textures = C.textures;

	this->VAO = new VertexArray(*C.VAO);
	this->VBO = new VertexBuffer(*C.VBO);
	this->EBO = new IndexBuffer(*C.EBO);
	this->VBL = new VertexBufferLayout(*C.VBL);
}

Mesh::~Mesh()
{/*删除内存中的Object，并不会删除VRAM中的数据*/
	delete VAO;
	delete VBO;
	delete EBO;
	delete VBL;
}

void Mesh::BindMesh() const
{
	VAO->Bind();
	EBO->Bind();
}

void Mesh::UnbindMesh() const
{
	VAO->Unbind();
	EBO->Unbind();
}


void Mesh::DeleteMesh()
{
	VAO->DeleteVertexArray();
	VBO->DeleteVertexBuffer();
	EBO->DeleteIndexBuffer();
	for (auto texture : textures) {
		texture.DeleteTexture();
	}
}

void Mesh::setupMesh()//本质是设置VAO
{
	VAO = new VertexArray();
	VBO = new VertexBuffer(&vertices[0], vertices.size() * sizeof(Vertex));
	EBO = new IndexBuffer(&indices[0], indices.size());
	VBL = new VertexBufferLayout();

	VBL->Push<float>(3);
	VBL->Push<float>(3);
	VBL->Push<float>(2);
	VBL->Push<float>(1);

	VAO->AddBuffer(*VBO,*VBL);
	VAO->Unbind();
}

