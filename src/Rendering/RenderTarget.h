#pragma once
#include "Texture.h"
class RenderTarget : public Texture
{
public:
	uint32 GetDepthTextureID() { return m_depthID; };
	uint32 GetFboTextureID() { return m_fbo; };
	void CreateFBO(string name, int width, int height);
	void CreateMultipleRenderTargetFrameBuffer(int width, int height);

	void RenderToTexture(bool clear);
	void EndRenderToTexture();
private:
	
	uint32 m_depthID;
	uint32 m_fbo;
	uint32 m_renderBuffer;
};

