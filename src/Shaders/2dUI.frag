#version 120
#extension GL_ARB_draw_buffers:enable
varying vec2 v_Texcoord;
varying float v_VertexID;
uniform sampler2D AlbedoSampler;
uniform sampler2D NormalSampler;
uniform sampler2D SpecularSampler;
uniform float u_Time;

void main()
{
	vec4 diffuse = texture2D(AlbedoSampler,v_Texcoord);
	gl_FragData[0] = vec4(diffuse);
}