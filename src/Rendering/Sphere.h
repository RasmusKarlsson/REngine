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
	int m_lats, m_longs;
	bool m_isInited;
	
	int m_numsToDraw;
};
