#pragma once
#include "Entity.h"
#include <iostream>
#include <string>
#include <lodepng.h>

class TerrainMesh : public Entity
{
public:
	TerrainMesh();
	~TerrainMesh();

	void CreatePatchFromHeightmap(Texture* texture, uint32 xStart, uint32 yStart, uint32 patchSize);

	ivec2 GetResolution() { return m_resolution; }

	void SetNrLods(int lods) { m_nrLods = lods; };
	int	 GetNrLods() { return m_nrLods; }

	void SetLod(int lod);
	int	 GetLod() { return m_currentLod;	}

private:

	ivec2 m_resolution;
	float m_size;

	float m_heightScale;

	vector<uint32> m_lodBufferObjects;
	vector<uint32> m_lodTriangleSize;
	vector<uint32> m_lodIndexSize;
	int m_currentLod;
	int m_nrLods;
};
