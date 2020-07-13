#include "Decal.h"

Decal::Decal()
{
	Create();
}

Decal::~Decal()
{
	if (m_cube)
		delete m_cube;
}

void Decal::Create()
{
	//Create a ref to cube
	m_cube = new Cube();
	m_vao = m_cube->GetVao();
	
}
