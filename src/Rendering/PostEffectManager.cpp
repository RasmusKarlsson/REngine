#include "PostEffectManager.h"

#include "Renderer.h"

//extern int SCREEN_WIDTH;
//extern int SCREEN_HEIGHT;

PostEffectManager::PostEffectManager()
{
	m_numberOfPostEffects = 0;
}

void PostEffectManager::InitializeFramebuffers()
{
	m_fbo[0] = new RenderTarget();
	m_fbo[0]->CreateFBO("framebuffer1a", 1280, 720);
	
	m_fbo[1] = new RenderTarget();
	m_fbo[1]->CreateFBO("framebuffer2a", 1280, 720);
}

void PostEffectManager::InitializePostEffects()
{
	m_postEffects.push_back(new PostEffect("src/shaders/fxaa.frag"));
	m_postEffects[0]->Enable();
	m_postEffects.push_back(new PostEffect("src/shaders/grayscale.frag"));
	m_postEffects[1]->Enable();
}

void PostEffectManager::BindFramebufferForRendering()
{
	m_numberOfPostEffects = 0;
	for (int i = 0; i < m_postEffects.size(); i++)
	{
		if (m_postEffects[i]->IsEnabled())
			m_numberOfPostEffects++;
	}
	if(m_numberOfPostEffects)
		m_fbo[0]->RenderToTexture(true);
}

void PostEffectManager::RenderPostEffects()
{
	if (!m_numberOfPostEffects) return;
	Renderer::SetRenderMode(RENGINE::RENDER_MODE::UI);

	int count = 0;
	for (int i = 0; i < m_postEffects.size(); i++)
	{
		if(!m_postEffects[i]->IsEnabled())
			continue;

		uint32 currentFBO = count % 2;
		uint32 nextFBO = (count + 1) % 2;
		bool lastPostEffect = count == (m_numberOfPostEffects - 1);

		Renderer::BindTexture(m_fbo[currentFBO]->GetTextureID(), 0);
		Renderer::BindTexture(m_fbo[currentFBO]->GetDepthTextureID(), 1);

		if (!lastPostEffect)
			m_fbo[nextFBO]->RenderToTexture(true);

		m_postEffects[i]->RenderPostEffect();
		count++;
	}
	m_fbo[0]->EndRenderToTexture();
	Renderer::RenderFullscreenQuad();
}
