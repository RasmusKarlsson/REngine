#version 120

uniform vec4 u_WireColor;

void main()
{
	gl_FragData[0] = u_WireColor;
}