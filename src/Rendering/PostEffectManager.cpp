#include "PostEffectManager.h"

#include "Renderer.h"


PostEffectManager::PostEffectManager()
{
	m_numberOfPostEffects = 0;
	m_frameBuffersInitialized = false;
}

void PostEffectManager::InitializeFramebuffers(uint32 width, uint32 height)
{
	DeleteFramebuffers();
	
	m_fbo[0] = new RenderTarget();
	m_fbo[0]->CreateFBO("framebuffer1a", width, height);
	
	m_fbo[1] = new RenderTarget();
	m_fbo[1]->CreateFBO("framebuffer2a", width, height);

	m_blurBuffers[0] = new RenderTarget();
	m_blurBuffers[0]->CreateFBO("blurBuffer0", width/2, height/2);

	m_blurBuffers[1] = new RenderTarget();
	m_blurBuffers[1]->CreateFBO("blurBuffer1", width / 4, height / 4);
	m_blurBuffers[2] = new RenderTarget();
	m_blurBuffers[2]->CreateFBO("blurBuffer2", width / 4, height / 4);

	m_frameBuffersInitialized = true;
}

void PostEffectManager::DeleteFramebuffers()
{
	if (m_fbo[0])
		delete m_fbo[0];
	
	if (m_fbo[1])
		delete m_fbo[1];

	if (m_blurBuffers[0])
		delete m_blurBuffers[0];
	if (m_blurBuffers[1])
		delete m_blurBuffers[1];
	if (m_blurBuffers[2])
		delete m_blurBuffers[2];

	m_frameBuffersInitialized = false;
}

void PostEffectManager::InitializePostEffectShaders()
{
	m_postEffects.push_back(new PostEffect("src/shaders/fxaa.frag"));
	m_postEffects[0]->Enable();
	m_postEffects.push_back(new PostEffect("src/shaders/grayscale.frag"));
	m_postEffects[1]->Enable();

	m_blurShader = new Shader("src/shaders/fullscreenBlur.vert", "src/shaders/fullscreenBlur.frag");
	m_downscaleShader = new Shader("src/shaders/fullscreenBlur.vert", "src/shaders/downscale.frag");
}

uint32 PostEffectManager::GetNumberOfPostEffects()
{
	uint32 nrPosteffects = 0;
	for (int i = 0; i < m_postEffects.size(); i++)
	{
		if (m_postEffects[i]->IsEnabled())
			nrPosteffects++;
	}
	return nrPosteffects;
}

void PostEffectManager::BindFramebufferForRendering()
{
	if(m_frameBuffersInitialized)
	{
		if (m_fbo[0]->GetWidth() != Renderer::m_windowWidth || m_fbo[0]->GetHeight() != Renderer::m_windowHeight)
			InitializeFramebuffers(Renderer::m_windowWidth, Renderer::m_windowHeight);
	}
	
	if(GetNumberOfPostEffects())
		m_fbo[0]->RenderToTexture(true);
}

void PostEffectManager::BindFbo(uint32 index, bool clear)
{
	Renderer::SetViewport(0, 0, m_fbo[index]->GetWidth(), m_fbo[index]->GetHeight());
	m_fbo[index]->RenderToTexture(index);
}

void PostEffectManager::RenderBlurBuffer()
{
	Renderer::SetRenderMode(RENGINE::RENDER_MODE::UI);
	Renderer::SetShader(m_blurShader->GetShaderID());


	vec2 texelSize;
	for (int i = 0; i < 3; i++)
	{
		Renderer::SetShader(m_blurShader->GetShaderID());
		if (i == 0)
		{
			Renderer::SetShader(m_downscaleShader->GetShaderID());
			m_blurBuffers[0]->RenderToTexture(true);
			Renderer::BindTexture(m_fbo[0]->GetTextureID(), 0);
			texelSize = vec2(1.0 / Renderer::GetWidth(), 1.0 / Renderer::GetHeight());
			Renderer::SetViewport(0, 0, m_blurBuffers[0]->GetWidth(), m_blurBuffers[0]->GetHeight());
		}
		else if(i==1)
		{
			m_blurBuffers[1]->RenderToTexture(true);
			Renderer::BindTexture(m_blurBuffers[0]->GetTextureID(), 0);
			texelSize = vec2(1.0 / Renderer::GetWidth(), 0.0);
			Renderer::SetViewport(0, 0, m_blurBuffers[1]->GetWidth(), m_blurBuffers[1]->GetHeight());
		}
		else
		{
			m_blurBuffers[2]->RenderToTexture(true);
			Renderer::BindTexture(m_blurBuffers[1]->GetTextureID(), 0);
			texelSize = vec2(0.0, 1.0 / Renderer::GetHeight());
			Renderer::SetViewport(0, 0, m_blurBuffers[2]->GetWidth(), m_blurBuffers[2]->GetHeight());
		}

		Renderer::UploadUniform2fv("u_texelSize", value_ptr(texelSize));
		Renderer::RenderFullscreenQuad();
	}
}

void PostEffectManager::RenderPostEffects()
{
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
