#version 120
#extension GL_EXT_gpu_shader4 : enable
varying vec2 v_Texcoord;
varying vec3 v_Color;

const vec2 quadVertices[4] = 
vec2[]( vec2( -1.0, -1.0 ), vec2( 1.0, -1.0 ), vec2( 1.0, 1.0 ), vec2( -1.0, 1.0 ) );

const vec2 quadUvs[4] = 
vec2[]( vec2( 0.0, 0.0 ), vec2( 1.0, 0.0 ), vec2( 1.0, 1.0 ), vec2( 0.0, 1.0 ) );

void main( ) {
	gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
	v_Texcoord = quadUvs[gl_VertexID];
	v_Color = vec3(1.0);
}