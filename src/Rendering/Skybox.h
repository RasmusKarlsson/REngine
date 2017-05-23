#pragma once
#include "Entity.h"

class Skybox : public Entity
{
public:
	Skybox();
	~Skybox();
	void Create();

private:

	Texture* m_equireTexture;
	Quad* m_skyMesh;
	Material* m_skyMaterial;
};
