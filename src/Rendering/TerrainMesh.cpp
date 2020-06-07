#pragma once

#include "Entity.h"
#include "TerrainMesh.h"
#include "MathHelpers.h"
#include "Renderer.h"

using namespace std;

int C_TERRAINLODS = 8;

float ReadHeightValue(vector<unsigned char> &imageData, int textureSize, int x, int y, int offsetX, int offsetY);

TerrainMesh::TerrainMesh()
{
	m_lodBufferObjects.resize(C_TERRAINLODS);
	m_lodTriangleSize.resize(C_TERRAINLODS);
	m_lodIndexSize.resize(C_TERRAINLODS);
	m_currentLod = 0;
}

float ReadHeightValue(vector<unsigned char> &imageData, int textureSize ,int x, int y, int offsetX, int offsetY)
{
	int xPos = clamp(offsetX + x,0,textureSize-1);
	int yPos = clamp(offsetY + y, 0, textureSize-1);
	int index = 4 * (yPos*textureSize + xPos);
	return static_cast<float>(imageData[index]);
}

void TerrainMesh::CreatePatchFromHeightmap(Texture* texture, uint32 xStart, uint32 yStart, uint32 patchSize)
{
	unsigned int width = patchSize;
	unsigned int depth = patchSize;

	//Heightmap too small for C_TERRAINLODS amount of lods?
	if ((int)log2((float)(patchSize - 1)) < C_TERRAINLODS)
	{
		//TODO: This is wrong
		C_TERRAINLODS = (int)log2((float)(patchSize - 1));
		m_lodBufferObjects.resize(C_TERRAINLODS);
		m_lodTriangleSize.resize(C_TERRAINLODS);
		m_lodIndexSize.resize(C_TERRAINLODS);
		SetNrLods(C_TERRAINLODS);
	}

	vector<unsigned char> imageData = texture->GetImageData();

	uint32 realSize = texture->GetWidth();

	m_resolution = ivec2(width, depth);
	m_heightScale = 0.75f;
	m_size = 0.1f;

	m_bbox.SetBoundingBox(vec3(0.0f), vec3(width, m_size*m_heightScale*256.0f, depth));

	vector<RENGINE::SVF_PNTC> vertexBuffer;

	for (uint32 x = 0; x < width; x++)
	{
		for (uint32 z = 0; z < depth; z++)
		{
			float height = m_size*ReadHeightValue(imageData, realSize, x, z, xStart, yStart);
			
			float x1 = m_size * ReadHeightValue(imageData, realSize, x + 1, z, xStart, yStart);
			float z1 = m_size * ReadHeightValue(imageData, realSize, x, z + 1, xStart, yStart);
			
			vec3 normal = normalize(
				cross(
					normalize(vec3(2.0f, 0.0f, height - x1)), 
					normalize(vec3(0.0f, 2.0f, height - z1))
				)
			);

			float fx = static_cast<float>(x);
			float fz = static_cast<float>(z);

			RENGINE::SVF_PNTC vertex;

			vertex.position = vec3(fx - patchSize * 0.5f, m_heightScale * height, fz - patchSize * 0.5f);
			vertex.normal = normal;
			vertex.texcoord = vec2(fx + xStart, fz + yStart);

			vertex.color = vec3(1.0);
			
			vertexBuffer.push_back(vertex);
		}
	}

	m_vao = Renderer::GenerateVertexArray();
	m_combinedVBO = Renderer::GenerateVertexBuffer(
		vertexBuffer.size(), 
		RENGINE::VF_PNTC_SIZE, 
		RENGINE::VF_PNTC, 
		vertexBuffer.data()
	);

	vector<int> lodIndices;

	//Triangle types: [\]  [/]  [\/] -> 0, 1, 2
	bool transpose = false;
	for (int lodIndex = C_TERRAINLODS - 1; lodIndex >= 0; lodIndex--)
	{
		int d = 0;
		int lod = MathHelpers::CounterToPowerOfTwo(lodIndex);
		lodIndices.resize((width - 1) / lod * (depth - 1) / lod * 6);
		
		for (uint32 y = 0; y < (depth - 1); y += lod)
		{
			for (uint32 x = 0; x < (width - 1); x += lod)
			{
				int idx = (y)*(width)+(x);

				//First Triangle
				lodIndices[d++] = idx;
				lodIndices[d++] = (idx + lod * width);
			
				//Criss cross trangle binding // transposed is [/]
				if (!transpose)
				{
					lodIndices[d++] = (idx + lod * width + lod);
					//Second Triangle
					lodIndices[d++] = idx;
				}
				else
				{
					lodIndices[d++] = (idx + lod);
					lodIndices[d++] = (idx + lod * width);
				}
				lodIndices[d++] = (idx + lod * width + lod);
				lodIndices[d++] = (idx + lod );
				
				transpose = !transpose;
			}
			transpose = !transpose;
		}

		m_lodTriangleSize[lodIndex] = lodIndices.size();
		m_lodIndexSize[lodIndex] = lodIndices.size();

		m_lodBufferObjects[lodIndex] = Renderer::GenerateIndexBuffer(lodIndices.size(), lodIndices.data());
	}

	m_triangleCount = (width - 1) * (depth - 1) * 6;
	m_indexSize = (width - 1) * (depth - 1) * 6;

	m_created = true;
}

void TerrainMesh::SetLod(int lod)
{
	if (m_currentLod == lod) return;
	m_currentLod = lod;
	m_triangleCount = m_lodTriangleSize[lod];
	m_indexSize = m_lodIndexSize[lod];
	Renderer::SetIndexBufferOnVertexArray(m_vao, m_lodBufferObjects[m_currentLod]);
}