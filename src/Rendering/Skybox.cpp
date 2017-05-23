///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Entity.h"
#include "Quad.h"
#include "Material.h"
#include "Skybox.h"

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
	m_skyMaterial = new Material();
	m_equireTexture = new Texture();
	m_equireTexture->CreateHDRFromFile("res/CubeMap/Sky.hdr");
}
