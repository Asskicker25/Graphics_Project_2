#pragma once

#include "Debugger.h"
#include <stbi/stb_image.h>

struct Image
{
	unsigned char* pixelData;
	int width;
	int height;
	int bpp;
	GLenum format;

	Image() : width{ 0 }, height{ 0 }, bpp{ 0 }, pixelData{ nullptr }
	{
	}
};


class Texture
{
private:
	Image texture;

public:

	static bool fileExists(const std::string& path);
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

