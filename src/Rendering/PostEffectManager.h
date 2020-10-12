#pragma once
#include <vector>
#include "PostEffect.h"
#include "RenderTarget.h"

class PostEffectManager
{
public:
	PostEffectManager();
	void InitializeFramebuffers(uint32 width, uint32 height);
	void DeleteFramebuffers();
	void InitializePostEffectShaders();
	void BindFramebufferForRendering();
	void BindFbo(uint32 index, bool clear);
	void RenderPostEffects();
	void RenderBlurBuffer();
	uint32 GetNumberOfPostEffects();
	RenderTarget* GetBlurRendertarget(uint32 layer) { return m_blurBuffers[layer]; };
private:
	std::vector<PostEffect*> m_postEffects;
	RenderTarget* m_fbo[2]					= {nullptr};
	RenderTarget* m_blurBuffers[3]			= {nullptr};
	uint32 m_numberOfPostEffects;
	Shader* m_blurShader = nullptr;
	Shader* m_downscaleShader = nullptr;
	bool m_frameBuffersInitialized			= false;
};