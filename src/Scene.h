#pragma once

#include "Camera.h"
#include "Terrain.h"
#include "Quad.h"
#include "Cube.h"
#include "TextMesh.h"



using namespace glm;
using namespace std;

class Scene
{
public:
	Scene();
	~Scene();

	mat4 SetModelViewProjectionMatrix(mat4 worldMatrix = mat4());

	Camera* GetCurrentCamera() { return m_currentCamera; };

	void AddEntity(Entity* entity);

	void UpdateScene(double dt);
	void RenderScene(double dt);

	Cube* m_cube;

private:
	vector<Entity*> m_EntityList;
	Camera* m_currentCamera;
	Terrain* m_terrain;
	Quad* m_cursorMesh;
	TextMesh* m_fpsMesh;
};
