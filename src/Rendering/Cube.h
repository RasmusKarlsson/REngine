#pragma once
#include <GL/glew.h>
#include "Entity.h"
#include <iostream>
#include <string>

class Cube : public Entity
{
public:
	Cube();
	~Cube();
	void Create();
};
