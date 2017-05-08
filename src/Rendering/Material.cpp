///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include "Renderer.h"
#include "Material.h"

Material::Material()
{
	m_name = "DefaultMaterial";

	m_diffuseTexture = NULL;
	m_normalTexture = NULL;
	m_ambientTexture = NULL;
	m_specularTexture = NULL;

	m_diffuseColor = vec4(1.0,1.0,1.0,1.0);
	m_ambientColor = vec4(0.0, 0.0, 0.0, 1.0);
	m_specularColor = vec4(1.0, 1.0, 1.0, 1.0);

	m_shaderID = Renderer::m_simpleShader;
}

Material::~Material()
{

}

void Material::BindTextures()
{
	if (m_diffuseTexture)
	{
		m_diffuseTexture->Bind(0);
	}

	if (m_normalTexture)
	{
		m_normalTexture->Bind(1);
	}

	if (m_ambientTexture)
	{
		m_ambientTexture->Bind(2);
	}

	if (m_specularTexture)
	{
		m_specularTexture->Bind(3);
	}
}
