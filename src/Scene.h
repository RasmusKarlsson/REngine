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

	void RenderWireFrame();
	void RenderBoundingBoxes();

	Cube* m_cube;

private:
	vector<Entity*> m_EntityList;
	Camera* m_currentCamera;
	Terrain* m_terrain;
	Quad* m_cursorMesh;
	TextMesh* m_fpsMesh;

	Texture* m_fbo1;
	Texture* m_fbo2;
	Quad* m_fullscreenQuad;
};
