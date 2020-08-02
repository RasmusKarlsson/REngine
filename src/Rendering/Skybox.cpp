///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Entity.h"
#include "Quad.h"
#include "Material.h"
#include "Skybox.h"
#include "Renderer.h"

Skybox::Skybox()
{
	Create();
}

Skybox::~Skybox()
{
}

void Skybox::Create()
{
	m_skyMesh = new Quad();
	m_material = new Material();
	
	m_skyMesh->SetMaterial(m_material);
	m_material->SetShader(Renderer::m_skyShader);
}

void Skybox::SetSkyTexture(Texture* texture)
{
	m_material->SetDiffuseTexture(texture);
}

Quad* Skybox::GetSkyEntity()
{
	return m_skyMesh;
}
