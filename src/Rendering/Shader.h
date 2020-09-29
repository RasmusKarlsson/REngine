#pragma once
#include <string>
#include <vector>

class Shader
{
public:
	Shader(std::string vertexPath, std::string fragmentPath);
	~Shader();

	void Delete();
	void Compile();

	int GetShaderID();
	void SetDirty() { m_dirty = true; };


	static std::vector<Shader*> m_shaders;
	static void SetShaderDirty(std::string shaderPath);
	
private:

	std::string m_vertexShaderPath;
	std::string m_fragmentShaderPath;

	int m_vertexShaderHandle = 0;
	int m_fragmentShaderHandle = 0;
	int m_shaderProgramHandle = 0;

	bool m_dirty = false;

};
