#version 120
varying vec2 v_Texcoord;
varying vec3 v_Color;
varying float v_VertexID;
varying vec3 v_Normal;
uniform sampler2D AlbedoSampler;
uniform sampler2D NormalSampler;
uniform sampler2D SpecularSampler;
uniform float u_Time;
varying vec3 v_skyDirection;
varying vec4 v_Position;

uniform mat4 u_invView;
uniform mat4 u_invProjection;

#define M_PI 3.14159265358979323846264338328

vec4 sample_equirectangular_map(vec3 dir, sampler2D sampler) 
{
	vec2 uv;
	uv.x = atan( dir.z, dir.x );
	uv.x += u_Time*0.004;
	uv.y = acos( dir.y );
	uv /= vec2( 2 * M_PI, M_PI );
	vec4 texture = texture2D( sampler, uv);
	texture.x = pow(texture.x,1.0/2.2);
	texture.y = pow(texture.y,1.0/2.2);
	texture.z = pow(texture.z,1.0/2.2);
 	return texture;
}


void main()
{
	vec3 skyDirection =  normalize(v_skyDirection);
	vec4 skyColor = sample_equirectangular_map(skyDirection,AlbedoSampler);
	gl_FragData[0] = vec4(skyColor);
}