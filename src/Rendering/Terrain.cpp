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
#include "Camera.h"

using namespace std;

const int PATCHSIZE = 64;
vec4 g_colorList[13];

Terrain::Terrain()
{
	m_textureLayers.resize(4);
	m_nrLods = 1;
	m_lodDistances.push_back(2000.0f);
	m_lodDistances.push_back(28.0f);
	m_lodDistances.push_back(50.0f);
	m_lodDistances.push_back(80.0f);
	m_lodDistances.push_back(200.0f);
	m_lodDistances.push_back(99990.0f);
	m_lastCamPos = vec3(INFINITY);

	g_colorList[0] = vec4(1.0, 0.0, 0.0, 1.0);
	g_colorList[1] = vec4(1.0, 1.0, 0.0, 1.0);
	g_colorList[2] = vec4(1.0, 0.0, 1.0, 1.0);
	g_colorList[3] = vec4(1.0, 1.0, 1.0, 1.0);
	g_colorList[4] = vec4(0.0, 0.0, 0.0, 1.0);
	g_colorList[5] = vec4(0.0, 1.0, 0.0, 1.0);
	g_colorList[6] = vec4(0.0, 0.0, 1.0, 1.0);
	g_colorList[7] = vec4(0.0, 1.0, 1.0, 1.0);
	g_colorList[8] = vec4(0.5, 0.0, 0.5, 1.0);
	g_colorList[9] = vec4(1.0, 0.5, 0.5, 1.0);
	g_colorList[10] = vec4(1.0, 0.5, 0.0, 1.0);
	g_colorList[11] = vec4(0.5, 0.0, 1.0, 1.0);
	g_colorList[12] = vec4(0.5, 0.5, 1.0, 1.0);
}

Terrain::~Terrain()
{
}

void Terrain::CreateTerrainMesh()
{
	m_terrainMesh = new TerrainMesh();
	if (!m_heightMap) return;
	//m_terrainMesh->CreateFromHeightmap(m_heightMap);
	int width = m_heightMap->GetWidth();
	int height = m_heightMap->GetHeight();

	m_nrPatchesWidth = (width  - 1) / PATCHSIZE;
	m_nrPatchesHeight = (height - 1) / PATCHSIZE;

	m_terrainPatches.resize(m_nrPatchesWidth*m_nrPatchesHeight);
	for (int x = 0; x < m_nrPatchesWidth; x++)
	{
		for (int y = 0; y < m_nrPatchesHeight; y++)
		{
			m_terrainPatches[y*m_nrPatchesWidth + x] = new TerrainMesh();
			m_terrainPatches[y*m_nrPatchesWidth + x]->CreatePatchFromHeightmap(m_heightMap, x*PATCHSIZE, y*PATCHSIZE, PATCHSIZE+1);
			m_terrainPatches[y*m_nrPatchesWidth + x]->SetPosition(vec3(x*PATCHSIZE - PATCHSIZE / 2, 0.0, y*PATCHSIZE - PATCHSIZE / 2));

			m_terrainPatches[y*m_nrPatchesWidth + x]->SetLod(x);
		}
	}
	m_nrLods = m_terrainPatches[0]->GetNrLods();
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
	glBindTexture(GL_TEXTURE_2D, m_splatMap->GetTextureID());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textureLayers[0]->GetTextureID());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_textureLayers[1]->GetTextureID());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_textureLayers[2]->GetTextureID());

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_normalMap->GetTextureID());
}

void Terrain::Render(Camera* camera)
{

	vec3 camPos = camera->GetCameraPosition();
	vec3 camDir = camera->GetCameraDirection();
	//UpdateLODSFromCameraDistance(camPos);

	for (int x = 0; x < m_nrPatchesWidth; x++)
	{
		for (int y = 0; y < m_nrPatchesHeight; y++)
		{
			vec3 patchDir = normalize(camPos - m_terrainPatches[y*m_nrPatchesWidth + x]->GetPosition());
		//	if (dot(patchDir, camDir) > 0.2) continue;
			int colorIndex = (x+y) % 13;
			glUniform4fv(glGetUniformLocation(Renderer::m_currentShader, "Color"), 1, value_ptr(g_colorList[colorIndex]));

//			UpdatePatchLod(m_terrainPatches[y*m_nrPatchesWidth + x], camPos);
			Renderer::Render(*m_terrainPatches[y*m_nrPatchesWidth + x]);
		}
	}
}

void Terrain::SetAllLods(int lods)
{
	for (int x = 0; x < m_nrPatchesWidth; x++)
	{
		for (int y = 0; y < m_nrPatchesHeight; y++)
		{
			m_terrainPatches[y*m_nrPatchesWidth + x]->SetLod(lods);
		}
	}
}

void Terrain::UpdateLODSFromCameraDistance(vec3 cameraPos)
{
	if (distance(cameraPos, m_lastCamPos) < 4.0f) return;
	m_lastCamPos = cameraPos;
	for (int x = 0; x < m_nrPatchesWidth; x++)
	{
		for (int y = 0; y < m_nrPatchesHeight; y++)
		{
			vec3 patchPos = m_terrainPatches[y*m_nrPatchesWidth + x]->GetWorldPosition();
			float patchDistance = distance(patchPos, cameraPos);
			int lod = m_nrLods-1;
			for (int i = 0; i < m_nrLods; i++)
			{
				if (patchDistance < m_lodDistances[i])
				{
					lod = i;
					break;
				}
			}
			
			m_terrainPatches[y*m_nrPatchesWidth + x]->SetLod(lod);
		}
	}
}

void Terrain::UpdatePatchLod(TerrainMesh* mesh, vec3 camPos)
{
	vec3 patchPos = mesh->GetWorldPosition();
	float patchDistance = distance(patchPos, camPos);
	int lod = m_nrLods - 1;
	for (int i = 0; i < m_nrLods; i++)
	{
		if (patchDistance < m_lodDistances[i])
		{
			lod = i;
			break;
		}
	}
	mesh->SetLod(lod);
}