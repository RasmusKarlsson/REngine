#version 120
//#extension GL_ARB_draw_buffers:enable
uniform sampler2D AlbedoSampler;
uniform sampler2D NormalSampler;
uniform sampler2D SpecularSampler;
uniform float u_Time;

uniform vec4 u_DiffuseColor;

varying vec2 v_Texcoord;
varying vec3 v_Color;
varying float v_VertexID;
varying vec3 v_Normal;



void main()
{
	vec4 color = vec4(v_Color.rgb,1.0) * u_DiffuseColor;
	vec4 diffuse = color*texture2D(AlbedoSampler,v_Texcoord);
	gl_FragColor = vec4(diffuse);
}