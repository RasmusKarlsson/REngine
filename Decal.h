///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once

#include "Entity.h"
#include "Cube.h"

#include "Renderer.h"
class Decal : public Entity
{
public:
	Decal();
	Decal::~Decal();
private:
	void Create();

	Cube* m_cube;
};