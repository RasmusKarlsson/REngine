#pragma once
#include <GL/glew.h>
#include <vector>

using namespace std;
typedef unsigned int uint32;

static uint32 s_boundTextures[32] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

class Texture
{
public:
	Texture();
	~Texture();
	Texture(string filename);

	void CreateFromPNG(string filename);
	void CreateFromHDR(string filename);

	uint32 GetWidth() { return m_textureWidth; };
	uint32 GetHeight() { return m_textureHeight; };

	vector<unsigned char> GetImageData() { return m_imageData; };

	uint32 GetTextureID() { return m_textureID; };

	void Bind(uint32 slot);


protected:
	uint32 m_textureWidth;
	uint32 m_textureHeight;

	string m_fileName;

	vector<unsigned char> m_imageData;

	uint32 m_textureID;
};
