#version 120

varying vec2 v_Texcoord;
varying vec3 v_Color;
varying float v_VertexID;
varying vec3 v_Normal;

uniform sampler2D AlbedoSampler;
uniform float u_Time;
uniform vec2 u_texelSize;

void main()
{
    vec3 col0 = texture2D(AlbedoSampler, v_Texcoord + vec2(u_texelSize.x,0.0)).xyz;
    vec3 col1 = texture2D(AlbedoSampler, v_Texcoord + vec2(0.0,u_texelSize.y)).xyz;
    vec3 col2 = texture2D(AlbedoSampler, v_Texcoord - vec2(u_texelSize.x,0.0)).xyz;
    vec3 col3 = texture2D(AlbedoSampler, v_Texcoord - vec2(0.0,u_texelSize.y)).xyz;

    vec3 col = (col0+col1+col2+col3) * 0.25;

    gl_FragColor = vec4( col.xyz, 1.0 );
}




