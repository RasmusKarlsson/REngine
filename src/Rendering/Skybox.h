#pragma once
#include "Entity.h"

class Skybox : public Entity
{
public:
	Skybox();
	~Skybox();
	Quad* GetSkyEntity();
	void Create();

private:

	Texture* m_equireTexture;
	Quad* m_skyMesh;
	Material* m_skyMaterial;
};
