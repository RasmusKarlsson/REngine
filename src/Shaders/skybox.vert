#version 120

attribute vec3 a_Position;

uniform mat4 u_invView;
uniform mat4 u_invProjection;

varying vec3 v_skyDirection;


void main()
{
    gl_Position = vec4(a_Position.xy,1.0,1.0);
	v_skyDirection =  mat3(u_invView) * (u_invProjection * gl_Position).xyz;
}

