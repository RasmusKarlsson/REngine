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

	ivec2 GetResolution() { return m_resolution; };

private:

	ivec2 m_resolution;
	float m_size;

	float m_heightScale;
};
