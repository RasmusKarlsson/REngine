#pragma once

#include "Texture.h"
#include <GL/glew.h>
#include <glm\glm.hpp>

#include <iostream>

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

	Texture GetDiffuseTexture()		{ return *m_diffuseTexture; };
	Texture GetNormalTexture()		{ return *m_normalTexture; };
	Texture GetAmbientTexture()		{ return *m_ambientTexture; };
	Texture GetSpecularTexture()	{ return *m_specularTexture; };

	void SetDiffuseColor(vec4 color) { m_diffuseColor = color; };
	vec4 GetDiffuseColor() { return m_diffuseColor; };

	void SetAmbientColor(vec4 color) { m_ambientColor = color; };
	vec4 GetAmbientColor() { return m_ambientColor; };

	void SetSpecularColor(vec4 color) { m_specularColor = color; };
	vec4 GetSpecularColor() { return m_specularColor; };

	void SetShader(GLuint shader) { m_shaderID = shader; };
	GLuint GetShader() { return m_shaderID; };

private:

	string m_name;

	Texture* m_diffuseTexture = NULL;
	Texture* m_normalTexture = NULL;
	Texture* m_ambientTexture = NULL;
	Texture* m_specularTexture = NULL;

	bool m_enabledDiffuse = false;
	bool m_enabledNormal = false;
	bool m_enabledAmbient = false;
	bool m_enabledSpecular = false;

	vec4 m_diffuseColor;
	vec4 m_ambientColor;
	vec4 m_specularColor;

	GLuint m_shaderID;

	int m_renderStyle;
};
