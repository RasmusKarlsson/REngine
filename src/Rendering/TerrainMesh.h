#pragma once
#include <GL/glew.h>
#include "Entity.h"
#include <iostream>
#include <string>
#include <lodepng.h>

class TerrainMesh : public Entity
{
public:
	TerrainMesh();
	~TerrainMesh();
	void Create(int width, int depth);

	void CreateFromHeightmap(Texture* texture);
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

	vector<GLuint> m_lodBufferObjects;
	vector<GLuint> m_lodTriangleSize;
	vector<GLuint> m_lodIndexSize;
	int m_currentLod;
	int m_nrLods;
};
