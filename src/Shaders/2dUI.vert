#version 120

//layout(location = 0) in vec3 vertexPosition_modelspace;
attribute vec2 a_Position;
attribute vec2 a_Texcoord;
attribute float a_DataIndex;
uniform float u_Time;

varying vec2 v_Texcoord;
varying float v_VertexID;

uniform mat4 u_WorldViewProjection;
uniform mat4 u_World;
uniform vec2 u_ScreenSize;
uniform vec2 u_ElementSize;
	
void main(){

    gl_Position = u_World*vec4(vec2(1.0,-1.0)+a_Position.xy,-1.0,1.0);
	v_Texcoord = a_Texcoord.xy;
}

