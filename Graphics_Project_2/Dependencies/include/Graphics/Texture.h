#pragma once

#include <stbi/stb_image.h>
#include "Image.h"

class Texture
{
private:
	Image texture;

public:

	unsigned int renderedID;
	std::string type;
	std::string path;
	
	Texture();
	Texture(const std::string& path);
	~Texture();

	void Bind();
	void Unbind();
	void SetTextureSlot(int slot = 0);
	void LoadTexture(const std::string& path);

	static void LoadImage(const char* path, Image& image);
	static void LoadImage(const char* path, GLFWimage& image);

};

