#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Entity.h"

class Sphere : public Entity
{
public:
	Sphere();
	~Sphere();
	void Init();
	void cleanup();
	void draw();

private:
	int lats, longs;
	bool isInited;
	
	int numsToDraw;
};
