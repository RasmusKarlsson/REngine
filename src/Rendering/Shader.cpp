
#include "Shader.h"

typedef unsigned int uint32;

#include "Renderer.h"
#include "RenderingAPI/RendererContext.h"

std::vector<Shader*> Shader::m_shaders;

Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
	m_vertexShaderPath = vertexPath;
	m_fragmentShaderPath = fragmentPath;
	m_dirty = false;

	Compile();

	m_shaders.push_back(this);
}

Shader::~Shader()
{
	auto it = m_shaders.begin();
	while (it != m_shaders.end())
	{
		auto curr = it++;
		if (*curr == this) {
			m_shaders.erase(curr);
		}
	}
}

void Shader::Delete()
{
	RendererContext::DeleteShader(m_vertexShaderHandle);
	m_vertexShaderHandle = 0;
	RendererContext::DeleteShader(m_fragmentShaderHandle);
	m_fragmentShaderHandle = 0;
	RendererContext::DeleteShaderProgram(m_shaderProgramHandle);
	m_shaderProgramHandle = 0;
}

void Shader::Compile()
{
	m_vertexShaderHandle = RendererContext::CreateVertexShader(Renderer::LoadShaderFile(m_vertexShaderPath));
	m_fragmentShaderHandle = RendererContext::CreateFragmentShader(Renderer::LoadShaderFile(m_fragmentShaderPath));

	m_shaderProgramHandle = RendererContext::CreateShaderProgram(m_vertexShaderHandle, m_fragmentShaderHandle);
}

int Shader::GetShaderID()
{
	if(m_dirty)
	{
		Delete();
		Compile();
		m_dirty = false;
	}
	return m_shaderProgramHandle;
}

void Shader::SetShaderDirty(std::string shaderPath)
{
	for each(Shader* shader in m_shaders)
	{
		if(shader->m_vertexShaderPath.find(shaderPath) || shader->m_fragmentShaderPath.find(shaderPath))
		{
			shader->SetDirty();
		}
	}
}

