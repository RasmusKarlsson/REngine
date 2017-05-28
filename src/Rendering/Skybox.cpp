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
	Delete();
}

void Skybox::Create()
{
	m_skyMesh = new Quad();
	m_material = new Material();
	m_equireTexture = new Texture();
	m_equireTexture->CreateHDRFromFile("res/CubeMap/Sky512.hdr");

	m_material->SetDiffuseTexture(m_equireTexture);
	m_skyMesh->SetMaterial(m_material);
	m_material->SetShader(Renderer::m_skyShader);
}

Quad* Skybox::GetSkyEntity()
{
	return m_skyMesh;
}
