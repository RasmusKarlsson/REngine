#pragma once

#include <Decal.h>

#include "Camera.h"
#include "Terrain.h"
#include "Quad.h"
#include "Cube.h"
#include "PostEffectManager.h"
#include "RenderTarget.h"
#include "SkyBox.h"
#include "TextMesh.h"

using namespace glm;
using namespace std;

class Scene
{
public:
	Scene();
	~Scene();

	Camera* GetCurrentCamera() { return m_currentCamera; };
	PostEffectManager* GetPosteffectManager() { return m_postEffectManager; };

	void AddEntity(Entity* entity);

	void UpdateScene(double dt);
	void RenderScene(double dt);

	void RenderWireFrame();
	void RenderBoundingBoxes();
	void RenderStats();

	Decal* m_cube;
	Material* m_debugFontMaterial;
	Texture* m_debugFontTexture;

private:
	vector<Entity*> m_EntityList3D;
	vector<Entity*> m_EntityListUI;
	PostEffectManager* m_postEffectManager;
	Camera* m_currentCamera;
	Terrain* m_terrain;
	Quad* m_cursorMesh;
	TextMesh* m_fpsMesh;

	RenderTarget* m_fbo1;
	RenderTarget* m_fbo2;
	Quad* m_fullscreenQuad;
	Quad* m_windowQuad;
	Material* m_windowMaterial;

	Skybox* m_skybox;
};
