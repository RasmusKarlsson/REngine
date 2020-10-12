#pragma once
#include <string>


class Stats
{
public:
	static int s_textureBounds;
	static int s_shaderBounds;
	static int s_vertexCount;
	static int s_indexCount;
	static int s_renderStyleChanges;

	static std::string s_shaderError;
	
	static void Reset()
	{
		s_textureBounds = 0;
		s_shaderBounds = 0;
		s_vertexCount = 0;
		s_indexCount = 0;
		s_renderStyleChanges = 0;
	}
};
