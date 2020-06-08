#version 120
//#extension GL_ARB_draw_buffers:enable
varying vec2 v_Texcoord;
uniform sampler2D AlbedoSampler;
uniform sampler2D NormalSampler;
uniform sampler2D SpecularSampler;
uniform float u_Time;

void main()
{
	vec4 diffuse = texture2D(AlbedoSampler,v_Texcoord);
	gl_FragColor = vec4(diffuse);
	//gl_FragColor = vec4(v_Texcoord,0.0,1.0);
}