#pragma once


class Mesh : public Entity
{
public:
	Mesh();
	~Mesh();
	void LoadMeshFromObjFile(string fileName);
};

