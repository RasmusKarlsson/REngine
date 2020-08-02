
#include "Renderer.h"
#include "RenderingAPI/RendererContext.h"
#include "PostEffect.h"

typedef unsigned int uint32;

PostEffect::PostEffect(std::string fragmentShaderPath)
{
	m_needsDepth = false;
	m_enabled = false;
	CreateShader(fragmentShaderPath);
}

void PostEffect::CreateShader(std::string fragmentShaderPath)
{
	uint32 fullscreenVertex = RendererContext::CreateVertexShader(Renderer::LoadShaderFile("src/shaders/fullscreenPass.vert"));
	uint32 fragmentShader = RendererContext::CreateFragmentShader(Renderer::LoadShaderFile(fragmentShaderPath));

	m_shader = RendererContext::CreateShaderProgram(fullscreenVertex, fragmentShader);
}

void PostEffect::RenderPostEffect()
{
	Renderer::SetRenderMode(RENGINE::RENDER_MODE::UI);
	Renderer::SetShader(m_shader);

	Renderer::RenderFullscreenQuad();
}
