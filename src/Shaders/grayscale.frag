#version 120

varying vec2 v_Texcoord;
varying vec3 v_Color;
varying float v_VertexID;
varying vec3 v_Normal;
uniform sampler2D AlbedoSampler;
uniform float u_Time;

void main()
{
	vec3 color = texture2D(AlbedoSampler, v_Texcoord).rgb;
	color = texture2D(AlbedoSampler, v_Texcoord+0.05*color.rb).rgb;
	vec3 luma = vec3( 0.2126, 0.7152 ,0.0722);
	gl_FragColor = vec4(color,1.0);
	//gl_FragColor = vec4(vec3(dot(color,luma)),1.0);

}




