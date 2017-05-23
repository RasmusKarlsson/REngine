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

static bool m_bRenderWire = false;

Scene::Scene()
{
	Renderer::CompileShaders();

	m_currentCamera = new OrbitCamera();
	m_currentCamera->SetOrbitDistance(5.0f);
	m_currentCamera->SetCameraPosition(vec3(0.0f, 5.0f, 5.0f));
	m_currentCamera->SetCameraTarget(vec3(0.0f, 5.0f, 0.0f));
	m_currentCamera->CreateProjectionMatrix(45.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 4000.0f);
	
	Texture* tex = new Texture();
	tex->CreateFromFile("res\\Textures\\splatmap.png");

	glEnable(GL_ALPHA_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Material* mat = new Material();
	mat->SetDiffuseTexture(tex);

	Quad* quad = new Quad();
	quad->SetName("quad");
	quad->SetMaterial(mat);
	quad->SetPosition(0.0f, 2.0f, 0.0f);
	AddEntity(quad);

	Quad* quad3 = new Quad();
	quad3->SetName("quad3");
	quad3->SetMaterial(mat);
	quad3->SetPosition(0.0f, 2.0f, 1.0f);
	AddEntity(quad3);


	Cube* cube = new Cube();
	cube->SetName("cube");
	cube->AddChild(quad3);
	quad->AddChild(cube);
	m_cube = cube;
	Material* skyMat = new Material();
	skyMat->SetDiffuseTexture(new Texture("res\\Textures\\skyboxTest.png"));
	cube->SetMaterial(skyMat);
	cube->SetPosition(0.0f, 5.0f, 0.0f);
	AddEntity(cube);

	m_terrain = new Terrain();
	
	m_terrain->SetHeightMap("res\\Textures\\heightmap257.png");
	m_terrain->SetSplatMap("res\\Textures\\splatmap257.png");
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




	Quad* skyMesh = new Quad();
	Material* skyMaterial = new Material();
	Texture* equireTexture = new Texture();
	equireTexture->CreateHDRFromFile("res/CubeMap/SkyWithGrass.hdr");

	skyMesh->SetMaterial(skyMaterial);
	skyMaterial->SetDiffuseTexture(equireTexture);
	skyMaterial->SetShader(Renderer::m_skyShader);
	AddEntity(skyMesh);




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

	static int lod = 0;

	if (Input::IsPressedOnce('1')) {
		lod--;
		if (lod < 0) lod = 0;
		m_terrain->GetTerrainEntity()->SetLod(lod);
	}
	if (Input::IsPressedOnce('2')) {
		lod++;
		if (lod > 5) lod = 5;
		m_terrain->GetTerrainEntity()->SetLod(lod);
	}

	if (Input::IsPressedOnce('W')) {
		m_bRenderWire = !m_bRenderWire;
	}

	if (Input::IsPressedOnce('C')) {
		if (!m_cube->IsDead())
		{
			m_cube->Kill();
		}
		else
		{
			m_cube = new Cube();
			m_cube->SetName("newCube");
			m_cube->SetPosition(0.0f, 2.0f, 0.0f);
			Material* mat = new Material();
			Texture* tex = new Texture();
			tex->CreateFromFile("res\\Textures\\splatmap.png");
			mat->SetDiffuseTexture(tex);
			m_cube->SetMaterial(mat);
			AddEntity(m_cube);
		}
	}

	m_currentCamera->UpdateCameraState(dt);
	m_currentCamera->CreateLookAtMatrix();

	for (auto it = begin(m_EntityList); it != end(m_EntityList);) {
		if ((*it)->IsDead()) {
			delete (*it);
			it = m_EntityList.erase(it);
		}
		else {
			++it;
		}
	}

	for (auto it = begin(m_EntityList); it != end(m_EntityList);++it) {
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
	glPolygonOffset(0.0, 0.0);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	m_terrain->BindTextures();
	m_terrain->UpdateUniforms();
	Renderer::Render(m_terrain->GetTerrainEntity(), SetModelViewProjectionMatrix(m_terrain->GetTerrainEntity()->GetWorldMatrix()));
	
	for (auto it = begin(m_EntityList); it != end(m_EntityList); ++it) {
		mat4 MVPmatrix = SetModelViewProjectionMatrix((*it)->GetWorldMatrix());
		(*it)->GetMaterial()->BindTextures();
		GLuint shader = (*it)->GetMaterial()->GetShader();
		Renderer::SetShader(shader);


		mat4 invView = glm::inverse(m_currentCamera->GetViewMatrix());
		mat4 invProj = glm::inverse(m_currentCamera->GetProjectionMatrix());
		glUniformMatrix4fv(glGetUniformLocation(Renderer::m_currentShader, "u_invView"), 1, GL_FALSE, value_ptr(invView));
		glUniformMatrix4fv(glGetUniformLocation(Renderer::m_currentShader, "u_invProjection"), 1, GL_FALSE, value_ptr(invProj));

		Renderer::Render(*it, MVPmatrix);
	}

	if (m_bRenderWire)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonOffset(4.0, 4.0);

		glLineWidth(2.0f);
		Renderer::SetShader(Renderer::m_whiteShader);
		Renderer::Render(m_terrain->GetTerrainEntity(), SetModelViewProjectionMatrix(m_terrain->GetTerrainEntity()->GetWorldMatrix()));

		for (auto it = begin(m_EntityList); it != end(m_EntityList); ++it) {
			mat4 MVPmatrix = SetModelViewProjectionMatrix((*it)->GetWorldMatrix());
			(*it)->GetMaterial()->BindTextures();
			Renderer::Render(*it, MVPmatrix);
		}
	}
	


}
