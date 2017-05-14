#pragma once

#include "TerrainMesh.h"

class Terrain
{
public:
	Terrain();
	~Terrain();

	void CreateTerrainMesh();
	void SetHeightMap(string filename);
	void SetSplatMap(string filename);
	void SetNormalMap(string filename);
	void SetSplatMapTexture(string filename, int layer);
	void UpdateUniforms();
	TerrainMesh* GetTerrainEntity() {return m_terrainMesh;}
	

	void BindTextures();

	void Render(int lod, mat4 wvpMatrix);

private:

	TerrainMesh* m_terrainMesh;
	Texture* m_heightMap;
	Texture* m_splatMap;
	Texture* m_normalMap;
	vector<Texture*> m_textureLayers;
	unsigned int m_width;
	unsigned int m_height;
	float m_waterHeight = 0.3f;

};
