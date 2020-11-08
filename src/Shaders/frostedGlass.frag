#version 120

uniform sampler2D AlbedoSampler;
uniform sampler2D NormalSampler;
uniform sampler2D SpecularSampler;
uniform float u_Time;

uniform vec4 u_DiffuseColor;

varying vec2 v_Texcoord;
varying vec4 v_NdcTexcoord;
varying vec3 v_Color;
varying float v_VertexID;
varying vec3 v_Normal;



void main()
{

	vec3 luma = vec3( 0.2126, 0.7152 ,0.0722);

	vec3 ndc = v_NdcTexcoord.xyz / v_NdcTexcoord.w; 
	vec2 screenCoords = ndc.xy * 0.5 + 0.5; 

	float noise = texture2D(NormalSampler,2.0*vec2(2.0,1.0)*v_Texcoord).r;
	float noise2 = texture2D(NormalSampler,2.0*vec2(2.0,1.0)*v_Texcoord+0.923155).r;
	vec3 noiseVec = vec3(noise,noise2,noise+noise2)-0.5;
	vec3 blurred = texture2D(AlbedoSampler,screenCoords-0.01*noiseVec.xy).rgb;
	vec3 clearred = texture2D(SpecularSampler,screenCoords).rgb;
	

	vec3 final = blurred;

	float gray = dot(luma,blurred);

	vec3 color = 1.5*vec3(0.5,0.2,0.5)*gray;

	gl_FragColor = vec4(0.96*final+0.03*noiseVec,1.0);


}