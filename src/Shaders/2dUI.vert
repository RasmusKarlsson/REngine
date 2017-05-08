#version 120

// Input vertex data, different for all executions of this shader.
//layout(location = 0) in vec3 vertexPosition_modelspace;
attribute vec3 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_Texcoord;
attribute vec3 a_Color;
attribute float a_DataIndex;
uniform float u_Time;

varying vec2 v_Texcoord;
varying float v_VertexID;
varying vec3 v_Normal;
varying vec3 v_Color;

uniform mat4 u_WorldViewProjection;
uniform mat4 u_World;
uniform vec2 u_ScreenSize;
uniform vec2 u_ElementSize;
	
void main(){

    gl_Position = u_World*vec4(vec2(1.0,-1.0)+a_Position.xy,-1.0,1.0);
    gl_Position.xy = gl_Position.xy;
	v_Texcoord = a_Texcoord.xy;
	v_Color = a_Color;
	v_Normal = a_Normal;
}

