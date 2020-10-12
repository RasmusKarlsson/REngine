#pragma once
#include <string>

#include "Shader.h"

typedef unsigned int uint32;

class PostEffect
{
public:
	PostEffect(std::string fragmentShaderPath);
	PostEffect(std::string vertexShaderPath, std::string fragmentShaderPath);
	~PostEffect();
	void RenderPostEffect();
	void Enable() { m_enabled = true; }
	void Disable() { m_enabled = false; }
	bool IsEnabled() { return m_enabled; }

private:
	Shader* m_shader;
	bool m_needsDepth;
	bool m_enabled;
};

