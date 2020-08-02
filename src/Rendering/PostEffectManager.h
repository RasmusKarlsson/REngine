#pragma once
#include <vector>
#include "PostEffect.h"
#include "RenderTarget.h"

class PostEffectManager
{
public:
	PostEffectManager();
	void InitializeFramebuffers();
	void InitializePostEffects();
	void BindFramebufferForRendering();
	void RenderPostEffects();
private:
	std::vector<PostEffect*> m_postEffects;
	RenderTarget* m_fbo[2];
	uint32 m_numberOfPostEffects;
};