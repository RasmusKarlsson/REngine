#pragma once

#include "TerrainMesh.h"
#include "Camera.h"

enum lodEdgeDirection
{
	NOLOD,
	SOUTH,
	WEST,
	NORTH,
	EAST,
	SOUTHWEST,
	NORTHWEST,
	NORTHEAST,
	SOUTHEAST
};

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
	TerrainMesh* GetTerrainEntity() { return m_terrainMesh; }
	TerrainMesh* GetTerrainPatch(int x, int z) { return m_terrainPatches[z*m_nrPatchesWidth + x]; }
	
	int GetNrPatchesWidth() {return m_nrPatchesWidth;}

	int GetNrPatchesHeight() {return m_nrPatchesHeight;}
	void BindTextures();

	void SetAllLods(int lods);
	void UpdateLODSFromCameraDistance(vec3 cameraPos);
	void UpdatePatchLod(TerrainMesh* mesh, vec3 camPos);

	void Render(mat4 wvpMatrix, Camera* camera);

private:

	TerrainMesh* m_terrainMesh;
	vector<TerrainMesh*> m_terrainPatches;
	int m_nrPatchesWidth;
	int m_nrPatchesHeight;

	vector<float> m_lodDistances;
	int m_nrLods;
	vec3 m_lastCamPos;

	Texture* m_heightMap;
	Texture* m_splatMap;
	Texture* m_normalMap;
	vector<Texture*> m_textureLayers;
	unsigned int m_width;
	unsigned int m_height;
	float m_waterHeight = 0.3f;

};
