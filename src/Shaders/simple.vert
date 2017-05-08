#version 120

// Input vertex data, different for all executions of this shader.
//layout(location = 0) in vec3 vertexPosition_modelspace;
attribute vec3 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_Texcoord;
attribute vec3 a_Color;
uniform float u_Time;

varying vec2 v_Texcoord;
varying float v_VertexID;
varying vec3 v_Normal;
varying vec3 v_Color;

uniform mat4 u_WorldViewProjection;

void main(){
/*
	mat4 rotmat = mat4(1.0);
	rotmat[0][0] = cos(u_Time);
	rotmat[0][2] = sin(u_Time);
	rotmat[2][0] = -sin(u_Time);
	rotmat[2][2] = cos(u_Time);*/
    gl_Position = u_WorldViewProjection * vec4(a_Position,1.0);
	v_Texcoord = a_Texcoord.xy;
	v_Color = a_Color;
	v_Normal = a_Normal;

}

