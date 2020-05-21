///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once

#include "Scene.h"
#include "Entity.h"
#include "Renderer.h"
#include "Quad.h"
#include "Input.h"
#include "Cube.h"
#include "Log.h"
#include "Sphere.h"
#include "SkyBox.h"
#include "TerrainMesh.h"
#include "MathHelpers.h"

#include "OrbitCamera.h"
#include "RenderTarget.h"
#include "Stats.h"
#include "WASDCamera.h"

extern double timeElapsed;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

static bool m_bRenderWire = false; 
static bool m_bRenderBBox = false; 
static bool m_bShowStats  = false;


Scene::Scene()
{
	Renderer::CompileShaders();

	Renderer::Enable(RENGINE::RENDER_FEATURE_DEPTH_TEST);

	Renderer::SetDepthFunction(RENGINE::DEPTH_TEST_LEQUAL);

	//Camera
	m_currentCamera = new OrbitCamera();
	m_currentCamera->SetOrbitDistance(5.0f);
	m_currentCamera->SetCameraPosition(vec3(0.0f, 5.0f, 5.0f));
	m_currentCamera->SetCameraTarget(vec3(0.0f, 5.0f, 0.0f));
	m_currentCamera->CreateProjectionMatrix(45.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);

	m_fbo1 = new RenderTarget();
	m_fbo1->CreateFBO("framebuffer1", SCREEN_WIDTH, SCREEN_HEIGHT);

	m_fbo2 = new RenderTarget();
	m_fbo2->CreateFBO("framebuffer2", SCREEN_WIDTH, SCREEN_HEIGHT);


	//Debug font
	m_debugFontTexture = new Texture("res/Textures/font2.png");
	m_debugFontMaterial = new Material();
	m_debugFontMaterial->SetDiffuseTexture(m_debugFontTexture);
	m_debugFontMaterial->SetShader(Renderer::m_textShader);

	m_fullscreenQuad = new Quad();

	Texture* tex = new Texture();
	tex->CreateFromPNG("res/Textures/splatmap.png");

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
	skyMat->SetDiffuseTexture(new Texture("res/Textures/skyboxTest.png"));
	cube->SetMaterial(skyMat);
	cube->SetPosition(0.0f, 5.0f, 0.0f);
	AddEntity(cube);

	m_terrain = new Terrain();
	
	m_terrain->SetHeightMap("res/Textures/heightmap257.png");
	m_terrain->SetSplatMap("res/Textures/splatmap257.png");
	m_terrain->SetNormalMap("res/Textures/normalmap.png");
	m_terrain->SetSplatMapTexture("res/Textures/sand.png",0);
	m_terrain->SetSplatMapTexture("res/Textures/grass.png",1);
	m_terrain->SetSplatMapTexture("res/Textures/stone.png",2);
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
	
	m_cursorMesh = new Quad();
	Material* cursorMat = new Material();
	cursorMat->SetShader(Renderer::m_textShader);
	Texture* cursorTex = new Texture();
	cursorTex->CreateFromPNG("res/Textures/cursor.png");
	cursorMat->SetDiffuseTexture(cursorTex);
	m_cursorMesh->SetScale((float)cursorTex->GetWidth() / SCREEN_WIDTH, (float)cursorTex->GetHeight() / SCREEN_HEIGHT, 1.0f);
	m_cursorMesh->SetMaterial(cursorMat);

	m_cursorMesh->SetRenderStyle(RENGINE::UI);
	AddEntity(m_cursorMesh);
	
	m_fpsMesh = new TextMesh();
	m_fpsMesh->SetText("FPS: 0.00");
	m_fpsMesh->SetMaterial(m_debugFontMaterial);
	m_fpsMesh->SetScale(8.0f / SCREEN_WIDTH, 8.0f / SCREEN_HEIGHT, 1.0f);
	m_fpsMesh->SetPosition(MathHelpers::PixelPosToWorldPos(10.0f, 30.0f));
	AddEntity(m_fpsMesh);
	


	Skybox* sky = new Skybox();
	AddEntity(sky->GetSkyEntity());

	Renderer::SetShader(Renderer::m_simpleShader);
}


Scene::~Scene()
{
}

void Scene::AddEntity(Entity* entity)
{
	char buffer[100];
	snprintf(buffer, 100, " %s \n", (entity->GetName()));
	Log::nlog(buffer);
	int newRenderStyle = entity->GetRenderStyle();
	for (auto it = begin(m_EntityList); it != end(m_EntityList); ++it) {
		//Insert the entity sorted by render style
		if ((*it)->GetRenderStyle() >= newRenderStyle)
		{
			m_EntityList.insert(it, entity);
			return;
		}
	}

	m_EntityList.push_back(entity);
}

void Scene::UpdateScene(double dt)
{
	Stats::Reset();
	
	static double counter = 0.0;
	counter = (1.0 / dt);
	std::string intAsString = std::to_string(counter);
	intAsString = intAsString.substr(0, 5);
	m_fpsMesh->UpdateTextBuffer(intAsString);
	if (Input::IsPressed('A')) {
		m_EntityList[2]->SetScale(vec3(1.0f, 2.0f, 1.0f));
	}
	if (Input::IsPressed('D'))
		m_EntityList[2]->AddRotation(vec3(0.01f)*vec3(1.0f, 1.0f, 2.0f));

	static int lod = 0;

	if (Input::IsPressedOnce('1')) {
		lod--;
		if (lod < 0) lod = 0;
		m_terrain->SetAllLods(lod);
	//	m_terrain->GetTerrainEntity()->SetLod(lod);
	}
	if (Input::IsPressedOnce('2')) {
		lod++;
		if (lod > 5) lod = 5;
		m_terrain->SetAllLods(lod);
	//	m_terrain->GetTerrainEntity()->SetLod(lod);
	}

	if (Input::IsPressedOnce('W')) {
		m_bRenderWire = !m_bRenderWire;
	}
	if (Input::IsPressedOnce('B')) {
		m_bRenderBBox = !m_bRenderBBox;
	}
	if (Input::IsPressedOnce('S')) {
		m_bShowStats = !m_bShowStats;
	}

	if (Input::IsPressedOnce('M')) {
		glEnable(GL_MULTISAMPLE);
	}
	if (Input::IsPressedOnce('N')) {
		glDisable(GL_MULTISAMPLE);
	}
	if (Input::IsPressedOnce('C')) {
		if (!m_cube->IsDead())
		{
			m_cube->Kill();
		}
		else
		{
			m_cube = new Cube();
			m_cube->SetPosition(0.0f, 2.0f, 0.0f);
			Material* mat = new Material();
			Texture* tex = new Texture();
			tex->CreateFromPNG("res/Textures/splatmap.png");
			mat->SetDiffuseTexture(tex);
			m_cube->SetMaterial(mat);
			AddEntity(m_cube);
		}
	}

	m_currentCamera->UpdateCameraState(static_cast<float>(dt));
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

	for (auto entity : m_EntityList) {
		if (entity->IsDirty())
		{
			entity->UpdateMatrices();
		}
	}

	m_cursorMesh->SetPosition(MathHelpers::PixelPosToWorldPos(Input::GetMousePosV()));

	Input::SetMouseWheelValue(0);
}

void Scene::RenderScene(double dt)
{
	Renderer::ClearBuffer();

	//m_fbo1->RenderToTexture(true);

	Renderer::SetViewMatrix(m_currentCamera->GetViewMatrix());
	Renderer::SetProjectionMatrix(m_currentCamera->GetProjectionMatrix());

	Renderer::SetShader(Renderer::m_terrainShader);
	m_terrain->BindTextures();
	m_terrain->UpdateUniforms();
	Renderer::SetRenderMode(RENGINE::SOLID);

	m_terrain->Render(m_currentCamera);

	for (auto entity : m_EntityList) 
	{
		mat4 invView = glm::inverse(m_currentCamera->GetViewMatrix());
		mat4 invProj = glm::inverse(m_currentCamera->GetProjectionMatrix());
		glUniformMatrix4fv(glGetUniformLocation(Renderer::m_currentShader, "u_invView"), 1, GL_FALSE, value_ptr(invView));
		glUniformMatrix4fv(glGetUniformLocation(Renderer::m_currentShader, "u_invProjection"), 1, GL_FALSE, value_ptr(invProj));

		Renderer::Render(*entity);
	}

	//m_fbo1->EndRenderToTexture();
	//Renderer::SetRenderMode(RENGINE::UI);
	//Renderer::SetShader(Renderer::m_showDepthShader);
	
	/*
	Renderer::SetRenderStyle(Entity::RENDERSTYLE_2D);
	Renderer::SetShader(Renderer::m_fullscreenShader);
	vec2 horizontal = vec2(1.0, 0.0);
	vec2 vertical = vec2(0.0, 1.0);
	vec2 direction;
	/*
	static int pingpong = 2;

	if (Input::IsPressedOnce('U')) {
		pingpong += 2;
	}
	
	for (int i = 0; i < pingpong; i++)
	{
		direction = (i % 2 == 0) ? horizontal : vertical;
		m_fbo2->RenderToTexture(true);
		glBindTexture(GL_TEXTURE_2D, m_fbo1->GetTextureID());
		glUniform2fv(glGetUniformLocation(Renderer::m_currentShader, "u_direction"), 1, value_ptr(direction));
		Renderer::Render(m_fullscreenQuad, glm::mat4());
		m_fbo2->EndRenderToTexture();
		swap(m_fbo1, m_fbo2);
	}
	*/

	/*
	m_fbo2->RenderToTexture(true);
	glBindTexture(GL_TEXTURE_2D, m_fbo1->GetTextureID());
	glUniform2fv(glGetUniformLocation(Renderer::m_currentShader, "u_direction"), 1, value_ptr(horizontal));
	Renderer::Render(m_fullscreenQuad, glm::mat4());
	m_fbo2->EndRenderToTexture();

	swap(m_fbo1, m_fbo2);

	m_fbo2->RenderToTexture(true);
	glBindTexture(GL_TEXTURE_2D, m_fbo1->GetTextureID());
	glUniform2fv(glGetUniformLocation(Renderer::m_currentShader, "u_direction"), 1, value_ptr(horizontal));
	Renderer::Render(m_fullscreenQuad, glm::mat4());
	m_fbo2->EndRenderToTexture();
	swap(m_fbo1, m_fbo2);
	*/
	/*
	m_fbo1->RenderToTexture(true);
	glBindTexture(GL_TEXTURE_2D, m_fbo2->GetTextureID());
	glUniform2fv(glGetUniformLocation(Renderer::m_currentShader, "u_direction"), 1, value_ptr(vertical));
	Renderer::Render(m_fullscreenQuad, glm::mat4());
	m_fbo1->EndRenderToTexture();*/



	/*
	glBindTexture(GL_TEXTURE_2D, m_fbo1->GetTextureID());

	//glUniform2fv(glGetUniformLocation(Renderer::m_currentShader, "u_direction"), 1, value_ptr(direction));
	Renderer::Render(m_fullscreenQuad, glm::mat4());
	glEnable(GL_DEPTH_TEST);

	*/

	//glBindTexture(GL_TEXTURE_2D, m_fbo1->GetTextureID());
	//Renderer::SetShader(Renderer::m_fullscreenShader);
	//Renderer::RenderFullscreenQuad();
	
	if (m_bRenderWire)
	{
		RenderWireFrame();
	}
	
	if (m_bRenderBBox)
	{
		RenderBoundingBoxes();
	}

	if(m_bShowStats)
	{
		RenderStats();
	}
}

void Scene::RenderWireFrame()
{
	Renderer::SetRenderMode(RENGINE::WIREFRAME);
	Renderer::SetOverrideRenderMode(true);
	Renderer::SetShader(Renderer::m_whiteShader);
	m_terrain->Render(m_currentCamera);

	for (auto entity : m_EntityList) {
		vec4 white = vec4(1.0);
		//glUniform4fv(glGetUniformLocation(Renderer::m_currentShader, "Color"), 1, value_ptr(white));
		Renderer::Render(*entity);
	}
	Renderer::SetOverrideRenderMode(false);
}

void Scene::RenderBoundingBoxes()
{
	Renderer::SetRenderMode(RENGINE::WIREFRAME);
	Renderer::SetShader(Renderer::m_whiteShader);

	Cube cube;
	for (auto entity : m_EntityList) {

		cube.SetPosition(entity->GetWorldPosition());
		cube.SetScale(entity->GetWorldScale() * entity->GetBoundingBox().GetBoundingScale());

		Renderer::Render(cube);
	}
}

void Scene::RenderStats()
{
	static TextMesh statsMesh;
	statsMesh.SetMaterial(m_debugFontMaterial);
	statsMesh.SetScale(8.0f / SCREEN_WIDTH, 8.0f / SCREEN_HEIGHT, 1.0f);

	Renderer::SetRenderMode(RENGINE::UI);
	Renderer::SetShader(Renderer::m_textShader);

	float height = 50.0f;

	statsMesh.SetPosition(MathHelpers::PixelPosToWorldPos(10.0f, height));
	statsMesh.UpdateTextBuffer("Entities:        " + std::to_string(m_EntityList.size())); statsMesh.UpdateMatrices();
	Renderer::Render(statsMesh); height += 10.0f;
	
	statsMesh.SetPosition(MathHelpers::PixelPosToWorldPos(10.0f, height));
	statsMesh.UpdateTextBuffer("Shader Binds:    " + std::to_string(Stats::s_shaderBounds)); statsMesh.UpdateMatrices();
	Renderer::Render(statsMesh); height += 10.0f;

	statsMesh.SetPosition(MathHelpers::PixelPosToWorldPos(10.0f, height));
	statsMesh.UpdateTextBuffer("Texture Binds:   " + std::to_string(Stats::s_textureBounds)); statsMesh.UpdateMatrices();
	Renderer::Render(statsMesh); height += 10.0f;

	statsMesh.SetPosition(MathHelpers::PixelPosToWorldPos(10.0f, height));
	statsMesh.UpdateTextBuffer("Render Styles:   " + std::to_string(Stats::s_renderStyleChanges)); statsMesh.UpdateMatrices();
	Renderer::Render(statsMesh); height += 10.0f;
	
	statsMesh.SetPosition(MathHelpers::PixelPosToWorldPos(10.0f, height));
	statsMesh.UpdateTextBuffer("Vertex Count:    " + std::to_string(Stats::s_vertexCount)); statsMesh.UpdateMatrices();
	Renderer::Render(statsMesh); height += 10.0f;
	statsMesh.SetPosition(MathHelpers::PixelPosToWorldPos(10.0f, height));
	
	statsMesh.UpdateTextBuffer("Index Count:     " + std::to_string(Stats::s_indexCount)); statsMesh.UpdateMatrices();
	Renderer::Render(statsMesh); height += 10.0f;


	
}