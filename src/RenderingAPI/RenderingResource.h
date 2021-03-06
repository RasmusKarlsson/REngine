#pragma once

typedef unsigned int uint32;

namespace RENGINE
{
	//vertex formats
	struct SVF_PNTC
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;
		glm::vec3 color;
	};

	struct SVF_PT
	{
		glm::vec3 position;
		glm::vec2 texcoord;
	};

	enum VA //Vertex Attribute
	{
		POSITION = 1,
		NORMAL	= 2,
		TEXCOORD = 4,
		COLOR	= 8
	};

	enum VA_POINTERS
	{
		POSITION_POINTER = 0,
		NORMAL_POINTER = 1,
		TEXCOORD_POINTER = 2,
		COLOR_POINTER = 3
	};
	
	const uint32 VF_PNTC = VA::POSITION | VA::NORMAL | VA::TEXCOORD | VA::COLOR;
	const uint32 VF_PNTC_SIZE = sizeof(SVF_PNTC);
	
	const uint32 VF_PT = VA::POSITION | VA::TEXCOORD;
	const uint32 VF_PT_SIZE = sizeof(SVF_PT);

	
	enum POLYGON_TYPE
	{
		POLYGON_TYPE_POINTS,
		POLYGON_TYPE_LINES,
		POLYGON_TYPE_LINE_LOOP,
		POLYGON_TYPE_LINE_STRIP,
		POLYGON_TYPE_TRIANGLES,
		POLYGON_TYPE_TRIANGLE_STRIP,
		POLYGON_TYPE_TRIANGLE_FAN,
		POLYGON_TYPE_QUADS,
		POLYGON_TYPE_QUAD_STRIP
	};

	enum INDEX_TYPE
	{
		INDEX_TYPE_BYTE,
		INDEX_TYPE_UNSIGNED_BYTE,
		INDEX_TYPE_SHORT,
		INDEX_TYPE_UNSIGNED_SHORT,
		INDEX_TYPE_INT,
		INDEX_TYPE_UNSIGNED_INT,
		INDEX_TYPE_FLOAT,
	};

	enum RENDER_MODE
	{
		DISABLED,
		SOLID,
		WIREFRAME,
		SOLIDWIREFRAME,
		OPACITY,
		ADD,
		UI
	};

	enum PIXEL_FORMAT
	{
		PIXEL_FORMAT_RGB,
		PIXEL_FORMAT_RGBA,
		PIXEL_FORMAT_RGB16F,
		PIXEL_FORMAT_RGB32F,
		PIXEL_FORMAT_RGBA16F,
		PIXEL_FORMAT_RGBA32F
	};

	enum PIXEL_TYPE
	{
		PIXEL_TYPE_UNSIGNED_BYTE,
		PIXEL_TYPE_FLOAT
	};

	enum TEXTURE_FILE_FORMAT
	{
		TEXTURE_FILE_FORMAT_PNG,
		TEXTURE_FILE_FORMAT_HDR,
	};
	
	enum TEXTURE_TYPE
	{
		TEXTURE_TYPE_TEXTURE_1D,
		TEXTURE_TYPE_TEXTURE_1D_ARRAY,
		TEXTURE_TYPE_TEXTURE_2D,
		TEXTURE_TYPE_TEXTURE_2D_ARRAY,
		TEXTURE_TYPE_TEXTURE_2D_MULTISAMPLE,
		TEXTURE_TYPE_TEXTURE_2D_MULTISAMPLE_ARRAY,
		TEXTURE_TYPE_TEXTURE_3D,
		TEXTURE_TYPE_TEXTURE_CUBE_MAP,
		TEXTURE_TYPE_TEXTURE_CUBE_MAP_ARRAY,
		TEXTURE_TYPE_TEXTURE_RECTANGLE
	};
	enum TEXTURE_PNAME
	{
		TEXTURE_PNAME_DEPTH_STENCIL_TEXTURE_MODE,
		TEXTURE_PNAME_TEXTURE_BASE_LEVEL,
		TEXTURE_PNAME_TEXTURE_COMPARE_FUNC,
		TEXTURE_PNAME_TEXTURE_COMPARE_MODE,
		TEXTURE_PNAME_TEXTURE_LOD_BIAS,
		TEXTURE_PNAME_TEXTURE_MIN_FILTER,
		TEXTURE_PNAME_TEXTURE_MAG_FILTER,
		TEXTURE_PNAME_TEXTURE_MIN_LOD,
		TEXTURE_PNAME_TEXTURE_MAX_LOD,
		TEXTURE_PNAME_TEXTURE_MAX_LEVEL,
		TEXTURE_PNAME_TEXTURE_SWIZZLE_R,
		TEXTURE_PNAME_TEXTURE_SWIZZLE_G,
		TEXTURE_PNAME_TEXTURE_SWIZZLE_B,
		TEXTURE_PNAME_TEXTURE_SWIZZLE_A,
		TEXTURE_PNAME_TEXTURE_WRAP_S,
		TEXTURE_PNAME_TEXTURE_WRAP_T,
		TEXTURE_PNAME_TEXTURE_WRAP_R
	};

	enum TEXTURE_PARAM
	{
		TEXTURE_PARAM_NEAREST,
		TEXTURE_PARAM_LINEAR,
		TEXTURE_PARAM_NEAREST_MIPMAP_NEAREST,
		TEXTURE_PARAM_LINEAR_MIPMAP_NEAREST,
		TEXTURE_PARAM_NEAREST_MIPMAP_LINEAR,
		TEXTURE_PARAM_LINEAR_MIPMAP_LINEAR,
		TEXTURE_PARAM_TEXTURE_MAG_FILTER,
		TEXTURE_PARAM_TEXTURE_MIN_FILTER,
		TEXTURE_PARAM_TEXTURE_WRAP_S,
		TEXTURE_PARAM_TEXTURE_WRAP_T,
		TEXTURE_PARAM_CLAMP,
		TEXTURE_PARAM_REPEAT,
	};

	enum DEPTH_TEST
	{
		DEPTH_TEST_NEVER,
		DEPTH_TEST_ALWAYS,
		DEPTH_TEST_LESS,
		DEPTH_TEST_LEQUAL,
		DEPTH_TEST_GREATER,
		DEPTH_TEST_GEQUAL,
		DEPTH_TEST_EQUAL,
		DEPTH_TEST_NOTEQUAL
	};

	enum RENDER_FEATURE
	{
		RENDER_FEATURE_DITHER,
		RENDER_FEATURE_MULTISAMPLE,
		RENDER_FEATURE_BLEND,
		RENDER_FEATURE_CULL_FACE,
		RENDER_FEATURE_DEBUG_OUTPUT,
		RENDER_FEATURE_DEBUG_OUTPUT_SYNCHRONOUS,
		RENDER_FEATURE_DEPTH_CLAMP,
		RENDER_FEATURE_DEPTH_TEST,
		RENDER_FEATURE_FRAMEBUFFER_SRGB,
		RENDER_FEATURE_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING,
		RENDER_FEATURE_SRGB,
		RENDER_FEATURE_LINE_SMOOTH,
		RENDER_FEATURE_POLYGON_OFFSET_FILL,
		RENDER_FEATURE_FILL,
		RENDER_FEATURE_POLYGON_OFFSET_LINE,
		RENDER_FEATURE_LINE,
		RENDER_FEATURE_POLYGON_OFFSET_POINT,
		RENDER_FEATURE_POINT,
		RENDER_FEATURE_POLYGON_SMOOTH,
		RENDER_FEATURE_PRIMITIVE_RESTART,
		RENDER_FEATURE_PRIMITIVE_RESTART_FIXED_INDEX,
		RENDER_FEATURE_MIN_SAMPLE_SHADING_VALUE,
		RENDER_FEATURE_SCISSOR_TEST,
		RENDER_FEATURE_STENCIL_TEST,
		RENDER_FEATURE_TEXTURE_CUBE_MAP_SEAMLESS
	};
}