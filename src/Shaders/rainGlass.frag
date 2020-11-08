#version 120

uniform sampler2D AlbedoSampler;
uniform sampler2D NormalSampler;
uniform sampler2D Sampler2;
uniform sampler2D Sampler3;
uniform float u_Time;

uniform vec4 u_DiffuseColor;

varying vec2 v_Texcoord;
varying vec4 v_NdcTexcoord;
varying vec3 v_Color;
varying float v_VertexID;
varying vec3 v_Normal;

vec2 invRes = vec2(1.0/1280.0,1.0/720.0);

float GetMonotonicRandomizedWindowRainTime(float offset, float speedRandom, float time)
{
	float noisedTime = (6.4 * time + 1.4 * sin(2.5 * (time+ offset)) + 0.205 * sin(7.3 * (time + offset)) + 0.081 * sin(12.3 * (time + offset))) * 0.167;
	return mix(time, noisedTime, clamp(speedRandom,0.0,1.0));
}

void main()
{

	vec3 luma = vec3( 0.2126, 0.7152 ,0.0722);

	vec3 ndc = v_NdcTexcoord.xyz / v_NdcTexcoord.w; 
	vec2 screenCoords = ndc.xy * 0.5 + 0.5; 
	
	vec2 noise = texture2D(NormalSampler,0.5*v_Texcoord).rg*2.0-1.0;
	
	vec2 texcoord = 1.6*vec2(2.0,1.0)*v_Texcoord;

	float rainSpeed = 0.37;
	float noiseStr = 0.1;
	vec2 rainIDTexcoord = vec2(texcoord.x,texcoord.y);
	float rainID = texture2D(Sampler3,rainIDTexcoord).a;
	vec2 rainUV = texcoord;

	rainUV.y -= GetMonotonicRandomizedWindowRainTime(rainID, 1.0, u_Time) * rainSpeed + rainID;

	

	/*
	vec3 rain = texture2D(Sampler3,rainUV+noiseStr*vec2(noise.x,0.0),-99.0).rgb;
	rain += texture2D(Sampler3,rainUV+noiseStr*vec2(noise.x,0.0)+vec2(2.0/8.0,0.4),-99.0).rgb;
	rain += texture2D(Sampler3,rainUV+noiseStr*vec2(noise.x,0.0)+vec2(6.0/8.0,0.9),-99.0).rgb;
	//float drops = texture2D(Sampler3,texcoord+noiseStr*vec2(noise.x,0.0)).b * rain.r;
	float drops = texture2D(Sampler3,texcoord).b * rain.r;
	drops += rain.g;
	*/

	vec3 rain = texture2D(Sampler3,rainUV).rgb;
	float dropsNW = texture2D(Sampler3,texcoord+vec2(0.0,0.0)).b;
	float dropsNE = texture2D(Sampler3,texcoord+vec2(invRes.x,0.0)).b;
	float dropsSW = texture2D(Sampler3,texcoord+vec2(0.0,invRes.y)).b;
	float dropsSE = texture2D(Sampler3,texcoord+vec2(invRes.x,invRes.y)).b;

	float right = 1.0*(dropsNE-dropsNW);
	float up = 1.0*(dropsSE-dropsNW);
	
	vec3 p0 = vec3(ndc.xy,0.0);
	vec3 p1 = vec3(ndc.xy+vec2(right,0.0),1.0);
	vec3 p2 = vec3(ndc.xy+vec2(0.0,up),1.0);
	
	//vec3 cNormal = 0.5+0.5*normalize(cross(vec3(1.0,right,0.0),vec3(up,1.0,0.0)));
	vec3 cNormal = 0.5+0.5*normalize(cross(p2-p0,p1-p0));

	/*
	vec3 X = dFdx(vec3(screenCoords.xy,0.5*drops));
	vec3 Y = dFdy(vec3(screenCoords.xy,0.5*drops));
	vec3 normal=0.5+0.5*normalize(cross(X,Y));
	*/
	vec3 blurred = texture2D(AlbedoSampler,screenCoords).rgb;
	//vec3 clearred = texture2D(Sampler2,screenCoords-0.1*normal.xy).rgb;


	//vec3 final = mix(clearred, blurred, drops);
	vec3 final = vec3(cNormal);
	gl_FragColor = vec4(0.96*vec3(final),1.0);


}