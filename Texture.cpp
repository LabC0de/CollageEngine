#include "Texture.h"
#include <stb_image.h>
#include <GL/glew.h>
#include "Debug.h"

std::unordered_map<std::string, Texture> Texture::map;
unsigned Texture::ctr = 0;

Texture::Texture(const char* fname)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fname, &width, &height, &channels, 0);
	ratio = static_cast<float>(width) / static_cast<float>(height);

	GLC(glGenTextures(1, &texture));
	GLC(glBindTexture(GL_TEXTURE_2D, texture));

	GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
	GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	switch (channels)
	{
	case 4:
		GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
		break;
	case 3:
		GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
		break;
	case 2:
		GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data));
		break;
	case 1:
		GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_R, GL_UNSIGNED_BYTE, data));
		break;
	}
	GLC(glGenerateMipmap(GL_TEXTURE_2D));
	stbi_image_free(data);
}

void Texture::use(int slot) const
{
	GLC(glActiveTexture(GL_TEXTURE0 + slot));
	GLC(glBindTexture(GL_TEXTURE_2D, texture));
}

void Texture::clear()
{
	GLC(glDeleteTextures(1, &texture));
}
