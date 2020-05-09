#pragma once
#include <GL/glew.h>
#include <vector>

using namespace std;
typedef unsigned int uint32;

static unsigned int s_boundTextures[32] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

class Texture
{
public:
	Texture();
	~Texture();
	Texture(string filename);

	void CreateFromFile(string filename);
	void CreateHDRFromFile(string filename);
	void CreateFBO(string name, int width, int height);
	void CreateMultipleRenderTargetFrameBuffer(int width, int height);

	unsigned int GetWidth() { return m_textureWidth; };
	unsigned int GetHeight() { return m_textureHeight; };

	vector<unsigned char> GetImageData() { return m_imageData; };

	void SetParameter(uint32 param, uint32 value);
	uint32 GetTextureID() { return m_textureID; };
	uint32 GetDepthTextureID() { return m_depthID; };

	void Bind(uint32 slot);

	void RenderToTexture(bool clear);
	void EndRenderToTexture();

private:
	unsigned int m_textureWidth;
	unsigned int m_textureHeight;

	string m_fileName;

	vector<unsigned char> m_imageData;

	uint32 m_textureID;
	uint32 m_depthID;
	uint32 m_fbo;
	uint32 m_renderBuffer;
};
