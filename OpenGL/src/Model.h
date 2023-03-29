#pragma once
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"


class Model
{
public:
	/*  ����   */
	Model(const std::string path, const std::string alphaDataPath)
	{
		loadAlpha(alphaDataPath);
		loadModel(path);
	}
	Model(const std::string path)
	{
		loadModel(path);
	}
	~Model();

	inline std::vector<Mesh> GetMeshs()const { return meshes; }
private:
	/*  ģ������  */
	std::vector<Mesh> meshes;
	std::vector<float> alpha;
	std::string directory;
	/*  ����   */
	void loadModel(std::string path);
	void loadAlpha(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		std::string typeName);
};

