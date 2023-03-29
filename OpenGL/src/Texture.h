#pragma once

#include "assimp/types.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width,m_Height,m_BPP/*bits per pixel*/;


public:

	std::string type;
	aiString path;


	Texture(const char* path, const std::string& directory, std::string _type, aiString _path, bool flip_vertically);
	Texture(const std::string path);
	void DeleteTexture() const;

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
	
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

};