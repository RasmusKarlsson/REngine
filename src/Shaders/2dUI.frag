#version 120
#extension GL_ARB_draw_buffers:enable
varying vec2 v_Texcoord;
varying vec3 v_Color;
varying float v_VertexID;
varying vec3 v_Normal;
uniform sampler2D AlbedoSampler;
uniform sampler2D NormalSampler;
uniform sampler2D SpecularSampler;
uniform float u_Time;

void main()
{
	vec4 color = vec4(v_Color.rgb,1.0);
	vec4 diffuse = texture2D(AlbedoSampler,v_Texcoord);
	//diffuse = vec4(v_Texcoord,0.0,1.0);
	gl_FragData[0] = vec4(diffuse);
}