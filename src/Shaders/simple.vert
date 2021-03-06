#version 120

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

    gl_Position = u_WorldViewProjection * vec4(a_Position,1.0);
	v_Texcoord = a_Texcoord.xy;
	v_Color = a_Color;
	v_Normal = a_Normal;

}

