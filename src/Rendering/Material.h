#pragma once

#include "Texture.h"
#include <glm\glm.hpp>

#include <iostream>

#include "Shader.h"

using namespace std;
using namespace glm;

class Material
{
public:
	Material();
	~Material();

	void SetName(string name) { m_name = name; };
	string GetName() { return m_name; };

	void BindTextures();

	void SetDiffuseTexture(Texture* texture) { m_diffuseTexture = texture;  m_enabledDiffuse = true; };
	void SetNormalTexture(Texture* texture)			{ m_normalTexture	= texture;  m_enabledNormal = true;};
	void SetAmbientTexture(Texture* texture) { m_ambientTexture = texture;   m_enabledAmbient = true; }
	void SetSpecularTexture(Texture* texture)		{ m_specularTexture = texture;   m_enabledSpecular = true;};

	Texture* GetDiffuseTexture()		{ return m_diffuseTexture; }
	Texture* GetNormalTexture()			{ return m_normalTexture; }
	Texture* GetAmbientTexture()		{ return m_ambientTexture; }
	Texture* GetSpecularTexture()		{ return m_specularTexture; }

	void SetDiffuseColor(vec4 color) { m_diffuseColor = color; };
	vec4 GetDiffuseColor() { return m_diffuseColor; };

	void SetAmbientColor(vec4 color) { m_ambientColor = color; };
	vec4 GetAmbientColor() { return m_ambientColor; };

	void SetSpecularColor(vec4 color) { m_specularColor = color; };
	vec4 GetSpecularColor() { return m_specularColor; };

	void SetShader(uint32 shader) { m_shaderID = shader; };
	uint32 GetShader() { return m_shaderID; };
	
	void SetShaderInstance(Shader* shader) {
		m_shaderInstance = shader;
		m_shaderID = 0;
	};
	Shader* GetShaderInstance() { return m_shaderInstance; };

private:

	string m_name;

	Texture* m_diffuseTexture = nullptr;
	Texture* m_normalTexture = nullptr;
	Texture* m_ambientTexture = nullptr;
	Texture* m_specularTexture = nullptr;

	bool m_enabledDiffuse = false;
	bool m_enabledNormal = false;
	bool m_enabledAmbient = false;
	bool m_enabledSpecular = false;

	vec4 m_diffuseColor;
	vec4 m_ambientColor;
	vec4 m_specularColor;

	uint32 m_shaderID;
	Shader* m_shaderInstance;

	int m_renderStyle;
};
