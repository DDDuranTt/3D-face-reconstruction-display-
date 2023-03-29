#include "Renderer.h"

#include <iostream>


void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const Model& model, Shader& shader, int u_texture) const
{
	shader.Bind();

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	std::vector<Mesh> meshs = model.GetMeshs();
	for (unsigned int i = 0; i < meshs.size(); ++i) {
		//---绑定vao、ibo---//
		meshs[i].BindMesh();

		//---绑定Texture---//
		for (unsigned int j = 0; j < meshs[i].textures.size(); ++j)
		{
			std::string number;
			std::string name = meshs[i].textures[j].type;

			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);

			meshs[i].textures[j].Bind(j);

			shader.SetUniform1i("u_Texture", j);
		}
		shader.SetUniform1i("u_Texture", u_texture);
		GLCall(glDrawElements(GL_TRIANGLES, meshs[i].indices.size(), GL_UNSIGNED_INT, nullptr));

	}
}


void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib,  Shader& shader) const 
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));//使用glDrawElements时，我们会使用当前绑定的索引缓冲对象中的索引进行绘制
	//GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));


}

void Renderer::Draw(const Model& model, Shader& shader) const
{
	shader.Bind();

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	std::vector<Mesh> meshs = model.GetMeshs();
	for (unsigned int i = 0; i < meshs.size(); ++i) {
		//---绑定vao、ibo---//
		meshs[i].BindMesh();

		//---绑定Texture---//
		for (unsigned int j = 0; j < meshs[i].textures.size(); ++j)
		{
			std::string number;
			std::string name = meshs[i].textures[j].type;

			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);

			meshs[i].textures[j].Bind(j);

			shader.SetUniform1i("u_Texture", j);
		}
		GLCall(glDrawElements(GL_TRIANGLES, meshs[i].indices.size(), GL_UNSIGNED_INT, nullptr));

	}
}
