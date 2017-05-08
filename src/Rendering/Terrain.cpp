///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include "Entity.h"
#include "TerrainMesh.h"
#include "Terrain.h"
#include "Texture.h"
#include "Renderer.h"

using namespace std;

Terrain::Terrain()
{
	m_textureLayers.resize(4);
}

Terrain::~Terrain()
{
}

void Terrain::CreateTerrainMesh()
{
	m_terrainMesh = new TerrainMesh();
	m_terrainMesh->CreateFromHeightmap(m_heightMap);
}

void Terrain::SetHeightMap(string filename)
{
	m_heightMap = new Texture();
	m_heightMap->CreateFromFile(filename);
}

void Terrain::SetSplatMap(string filename)
{
	m_splatMap = new Texture();
	m_splatMap->CreateFromFile(filename);
}

void Terrain::SetNormalMap(string filename)
{
	m_normalMap = new Texture();
	m_normalMap->CreateFromFile(filename);
}

void Terrain::SetSplatMapTexture(string filename, int layer)
{
	m_textureLayers[layer] = new Texture();
	m_textureLayers[layer]->CreateFromFile(filename);
}
void Terrain::UpdateUniforms()
{
	glUniform1f(glGetUniformLocation(Renderer::m_currentShader, "u_waterHeight"), m_waterHeight);

	mat4 worldMatrix = m_terrainMesh->GetWorldMatrix();
	glUniformMatrix4fv(glGetUniformLocation(Renderer::m_currentShader, "u_World"), 1, GL_FALSE, value_ptr(worldMatrix));
}
void Terrain::BindTextures()
{
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, m_splatMap->GetID());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textureLayers[0]->GetID());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_textureLayers[1]->GetID());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_textureLayers[2]->GetID());

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_normalMap->GetID());
}
