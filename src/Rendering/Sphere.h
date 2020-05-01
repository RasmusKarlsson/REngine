#pragma once

#include "Entity.h"

class Sphere : public Entity
{
public:
	Sphere();
	void Create();
	//void cleanup();
	//void draw();

private:
	int lats, longs;
	bool isInited;
	
	int numsToDraw;
};
