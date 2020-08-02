#pragma once
#include "Entity.h"

class Skybox : public Entity
{
public:
	Skybox();
	~Skybox();
	Quad* GetSkyEntity();
	void Create();
	void SetSkyTexture(Texture* texture);

private:

	Texture* m_equireTexture;
	Quad* m_skyMesh;
	Material* m_skyMaterial;
};
