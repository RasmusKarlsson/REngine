///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include "Renderer.h"

#include "Scene.h"
#include "Quad.h"
#include "Input.h"
#include "Cube.h"
#include "Sphere.h"
#include "TerrainMesh.h"
#include "MathHelpers.h"

#include "OrbitCamera.h"
#include "WASDCamera.h"

extern double timeElapsed;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

Scene::Scene()
{
	Renderer::CompileShaders();

	m_currentCamera = new OrbitCamera();
	m_currentCamera->SetOrbitDistance(5.0f);
	m_currentCamera->SetCameraPosition(vec3(0.0f, 0.0f, 5.0f));
	m_currentCamera->SetCameraTarget(vec3(0.0f, 0.0f, 0.0f));
	m_currentCamera->CreateProjectionMatrix(45.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

	Texture* tex = new Texture();
	tex->CreateFromFile("res\\Textures\\splatmap.png");

	glEnable(GL_ALPHA_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Material* mat = new Material();
	mat->SetDiffuseTexture(tex);

	Quad* quad = new Quad();
	quad->SetMaterial(mat);
	quad->SetPosition(0.0f, 2.0f, 0.0f);
	AddEntity(quad);


	Cube* cube = new Cube();
	cube->SetMaterial(mat);
	cube->SetPosition(0.0f, 5.0f, 0.0f);
	AddEntity(cube);

	m_terrain = new Terrain();
	
	m_terrain->SetHeightMap("res\\Textures\\heightmap65.png");
	m_terrain->SetSplatMap("res\\Textures\\splatmap.png");
	m_terrain->SetNormalMap("res\\Textures\\normalmap.png");
	m_terrain->SetSplatMapTexture("res\\Textures\\sand.png",0);
	m_terrain->SetSplatMapTexture("res\\Textures\\grass.png",1);
	m_terrain->SetSplatMapTexture("res\\Textures\\stone.png",2);
	m_terrain->CreateTerrainMesh();
	TerrainMesh* terrainEntity = m_terrain->GetTerrainEntity();
	terrainEntity->SetPosition(-(float)terrainEntity->GetResolution().x / 2, -3.0f, -(float)terrainEntity->GetResolution().y / 2);
	
	Quad* textQuad = new Quad();
	Quad* textQuad2 = new Quad();
	Material* textMat = new Material();
	textMat->SetDiffuseTexture(tex);
	textMat->SetShader(Renderer::m_textShader);
	textQuad->SetMaterial(textMat);
	textQuad2->SetMaterial(textMat);
	textQuad->SetScale(8.0f/ SCREEN_WIDTH,8.0f/ SCREEN_HEIGHT,1.0f);
	textQuad->SetPosition(MathHelpers::PixelPosToWorldPos(10.0f,10.0f));
	AddEntity(textQuad);
	//AddEntity(textQuad2);
	
	m_cursorMesh = new Quad();
	Material* cursorMat = new Material();
	cursorMat->SetShader(Renderer::m_textShader);
	Texture* cursorTex = new Texture();
	cursorTex->CreateFromFile("res\\Textures\\cursor.png");
	cursorMat->SetDiffuseTexture(cursorTex);
	m_cursorMesh->SetScale((float)cursorTex->GetWidth() / SCREEN_WIDTH, (float)cursorTex->GetHeight() / SCREEN_HEIGHT, 1.0f);
	m_cursorMesh->SetMaterial(cursorMat);
	AddEntity(m_cursorMesh);

	m_fpsMesh = new TextMesh();
	Material* fontMat = new Material();
	Texture* fontTex = new Texture();
	fontMat->SetShader(Renderer::m_textShader);
	fontTex->CreateFromFile("res\\Textures\\font.png");
	fontMat->SetDiffuseTexture(fontTex);
	m_fpsMesh->SetMaterial(fontMat);
	m_fpsMesh->SetText("1000000");
	m_fpsMesh->CreateMesh();
	m_fpsMesh->UpdateText("1");
	m_fpsMesh->SetScale(8.0f / SCREEN_WIDTH, 8.0f / SCREEN_HEIGHT, 1.0f);
	m_fpsMesh->SetPosition(MathHelpers::PixelPosToWorldPos(10.0f, 30.0f));
	AddEntity(m_fpsMesh);

	Renderer::SetShader(Renderer::m_simpleShader);
}


Scene::~Scene()
{
}

mat4 Scene::SetModelViewProjectionMatrix(mat4 worldMatrix)
{
	return m_currentCamera->GetProjectionMatrix() * m_currentCamera->GetViewMatrix() * worldMatrix;
}

void Scene::AddEntity(Entity* entity)
{
	m_EntityList.push_back(entity);
}

void Scene::UpdateScene(double dt)
{
	static double counter = 0.0;
	counter = 1.0/dt;
	std::string varAsString = std::to_string(counter);
	m_fpsMesh->UpdateText(varAsString);
	if (Input::IsPressed('A')) {
		m_EntityList[0]->SetScale(vec3(1.0f, 2.0f, 1.0f));
	}
	if (Input::IsPressed('D'))
		m_EntityList[0]->SetRotation(vec3(timeElapsed*1.4f, timeElapsed, 0.0f));

	m_currentCamera->UpdateCameraState(dt);
	m_currentCamera->CreateLookAtMatrix();

	for (auto it = begin(m_EntityList); it != end(m_EntityList); ++it) {

		if ((*it)->IsDirty())
		{
			(*it)->UpdateMatrices();
		}
	}


	m_cursorMesh->SetPosition(MathHelpers::PixelPosToWorldPos(Input::GetMousePosV()));

	Input::SetMouseWheelValue(0);
}

void Scene::RenderScene(double dt)
{
	Renderer::ClearBuffer();
	Renderer::SetShader(Renderer::m_terrainShader);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	m_terrain->BindTextures();
	m_terrain->UpdateUniforms();
	Renderer::Render(m_terrain->GetTerrainEntity(), SetModelViewProjectionMatrix(m_terrain->GetTerrainEntity()->GetWorldMatrix()));
	
	for (auto it = begin(m_EntityList); it != end(m_EntityList); ++it) {
		mat4 MVPmatrix = SetModelViewProjectionMatrix((*it)->GetWorldMatrix());
		(*it)->GetMaterial()->BindTextures();
		GLuint shader = (*it)->GetMaterial()->GetShader();
		Renderer::SetShader(shader);
		Renderer::Render(*it, MVPmatrix);
	}
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glLineWidth(1.0f);
	Renderer::Render(m_terrain->GetTerrainEntity(), SetModelViewProjectionMatrix());

	Renderer::SetShader(Renderer::m_simpleShader);
	for (auto it = begin(m_EntityList); it != end(m_EntityList); ++it) {
		mat4 MVPmatrix = SetModelViewProjectionMatrix((*it)->GetWorldMatrix());
		(*it)->GetMaterial()->BindTextures();
		Renderer::Render(*it, MVPmatrix);
	}
	


}
