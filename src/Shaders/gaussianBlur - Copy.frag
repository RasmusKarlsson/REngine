#version 120

varying vec2 v_Texcoord;
varying vec3 v_Color;
varying float v_VertexID;
varying vec3 v_Normal;
uniform sampler2D AlbedoSampler;
uniform sampler2D DepthSampler;
uniform float u_Time;
uniform vec2 u_direction;

vec4 blur13(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.411764705882353) * direction;
  vec2 off2 = vec2(3.2941176470588234) * direction;
  vec2 off3 = vec2(5.176470588235294) * direction;
  color += texture2D(image, uv) * 0.1964825501511404;
  color += texture2D(image, uv + (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv - (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv + (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv - (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv + (off3 / resolution)) * 0.010381362401148057;
  color += texture2D(image, uv - (off3 / resolution)) * 0.010381362401148057;
  return color;
}

float linearize_depth(float d,float zNear,float zFar)
{
	d = 2.0*d -1.0;
    return zNear * zFar / (zFar + d * (zNear - zFar));
}

void main()
{
	vec2 texSize = vec2(1.0)/vec2(1280.0,720.0);
	
	vec3 NW = texture2D(AlbedoSampler,v_Texcoord + texSize*vec2(-1.0,1.0)).rgb;
	vec3 NE = texture2D(AlbedoSampler,v_Texcoord + texSize*vec2(1.0,1.0)).rgb;
	vec3 SW = texture2D(AlbedoSampler,v_Texcoord + texSize*vec2(-1.0,-1.0)).rgb;
	vec3 SE = texture2D(AlbedoSampler,v_Texcoord + texSize*vec2(1.0,-1.0)).rgb;
	
	vec3 N = texture2D(AlbedoSampler,v_Texcoord + texSize*vec2(0.0,1.0)).rgb;
	vec3 W = texture2D(AlbedoSampler,v_Texcoord + texSize*vec2(-1.0,0.0)).rgb;
	vec3 S = texture2D(AlbedoSampler,v_Texcoord + texSize*vec2(0.0,-1.0)).rgb;
	vec3 E = texture2D(AlbedoSampler,v_Texcoord + texSize*vec2(1.0,0.0)).rgb;


	//vec3 diffuse = blur13(AlbedoSampler,v_Texcoord,texSize,u_direction).rgb;
	float depth = texture2D(DepthSampler,v_Texcoord).r;
	depth = linearize_depth(depth,0.1,1000.0);
	vec3 diffuse = 0.1*vec3(NW+NW+SW+SE + N+W+S+E);
	gl_FragColor = vec4(vec3(depth),1.0);

}