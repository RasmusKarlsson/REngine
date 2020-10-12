
#include "Renderer.h"
#include "PostEffect.h"

typedef unsigned int uint32;

PostEffect::PostEffect(std::string fragmentShaderPath)
{
	m_needsDepth = false;
	m_enabled = false;
	m_shader = new Shader("src/shaders/fullscreenPass.vert", fragmentShaderPath);
}
PostEffect::PostEffect(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	m_needsDepth = false;
	m_enabled = false;
	m_shader = new Shader(vertexShaderPath, fragmentShaderPath);
}

PostEffect::~PostEffect()
{
	delete m_shader;
}

void PostEffect::RenderPostEffect()
{
	Renderer::SetRenderMode(RENGINE::RENDER_MODE::UI);
	Renderer::SetShader(m_shader->GetShaderID());

	Renderer::RenderFullscreenQuad();
}
