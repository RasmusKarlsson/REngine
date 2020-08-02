#pragma once
#include <string>

typedef unsigned int uint32;

class PostEffect
{
public:
	PostEffect(std::string fragmentShaderPath);
	void CreateShader(std::string fragmentShaderPath);
	void RenderPostEffect();
	void Enable() { m_enabled = true; }
	void Disable() { m_enabled = false; }
	bool IsEnabled() { return m_enabled; }

private:
	uint32 m_shader;
	bool m_needsDepth;
	bool m_enabled;
};

