#version 120

uniform sampler2D AlbedoSampler;
uniform sampler2D NormalSampler;
uniform sampler2D SpecularSampler;
uniform float u_Time;

uniform vec4 u_DiffuseColor;

varying vec2 v_Texcoord;
varying vec2 v_NdcTexcoord;
varying vec3 v_Color;
varying float v_VertexID;
varying vec3 v_Normal;



void main()
{
	float r = texture2D(AlbedoSampler,v_NdcTexcoord-0.003).r;
	float g = texture2D(AlbedoSampler,v_NdcTexcoord).g;
	float b = texture2D(AlbedoSampler,v_NdcTexcoord+0.004).b;
	vec4 color = vec4(0.96*r,0.96*g,0.96*b,1.0);
	gl_FragColor = color;
	float normal = texture2D(NormalSampler,2.1*v_Texcoord+vec2(0.01*u_Time,0.0)).r;
	vec3 blurred = texture2D(AlbedoSampler,v_NdcTexcoord).rgb;
	vec3 clearred = texture2D(SpecularSampler,v_NdcTexcoord).rgb;
	vec3 final = normal*blurred + (1.0-normal)*clearred;
	gl_FragColor = vec4(0.96*final,1.0);
	//gl_FragColor = vec4(normal,0.0,1.0);
}