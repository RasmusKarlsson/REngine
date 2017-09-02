#version 120

// Input vertex data, different for all executions of this shader.
//layout(location = 0) in vec3 vertexPosition_modelspace;
attribute vec4 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_Texcoord;
attribute vec4 a_Color;
uniform float u_Time;

varying vec2 v_Texcoord;
varying float v_VertexID;
varying vec3 v_Normal;
varying vec4 v_Color;
varying float v_positionY;
uniform float u_waterHeight;

varying float v_fogAmount;

uniform mat4 u_WorldViewProjection;

#define FOG_START 120.0f
#define FOG_END 150.0f

float fogFactorLinear( const float dist, const float start, const float end ) {
  return 1.0 - clamp((end - dist) / (end - start), 0.0, 1.0);
}

void main(){

    gl_Position = u_WorldViewProjection * a_Position;
	float fogDistance = length(gl_Position.xyz);
	v_fogAmount = fogFactorLinear(fogDistance, FOG_START, FOG_END);
	float st = smoothstep(a_Position.y,0.0,u_waterHeight);
	v_positionY = st;
   // gl_Position.w = 1.0;
	v_Texcoord = a_Texcoord.xy;
	v_Color = 0.5+0.5*a_Color;
	v_Normal = a_Normal;

}

