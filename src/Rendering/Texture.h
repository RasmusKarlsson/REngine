#pragma once
#include <GL/glew.h>
#include <vector>
#include "RenderingAPI/RendererContext.h"

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

	uint32 GetTextureID();
	void SetDirty() { m_dirty = true; };
	void SetKeepImageData() { m_keepImageData = true; };

	void Bind(uint32 slot);
	static std::vector<Texture*> m_textures;
	static void SetTextureDirty(std::string texturePath);
	static bool IsImageFileExtension(std::string texturePath);
	
protected:
	uint32 m_textureWidth;
	uint32 m_textureHeight;
	string m_fileName;
	vector<unsigned char> m_imageData;
	uint32 m_textureID = 0;
	uint32 m_textureFileFormat = RENGINE::TEXTURE_FILE_FORMAT_PNG;
	bool m_keepImageData = false;
	bool m_dirty = false;
};
