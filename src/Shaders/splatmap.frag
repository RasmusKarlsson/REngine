#version 130

varying vec2 v_Texcoord;
varying vec4 v_Color;
varying float v_VertexID;
varying vec3 v_Normal;

varying float v_positionY;

uniform sampler2D Sampler0;
uniform sampler2D Sampler1;
uniform sampler2D Sampler2;
uniform sampler2D Sampler3;
uniform sampler2D Sampler4;
uniform float u_Time;

vec4 hash4( vec2 p ) { return fract(sin(vec4( 1.0+dot(p,vec2(37.0,17.0)), 
                                              2.0+dot(p,vec2(11.0,47.0)),
                                              3.0+dot(p,vec2(41.0,29.0)),
                                              4.0+dot(p,vec2(23.0,31.0))))*103.0); }

vec4 textureNoTile( sampler2D samp, in vec2 uv )
{
    ivec2 iuv = ivec2( floor( uv ) );
     vec2 fuv = fract( uv );

    // generate per-tile transform
    vec4 ofa = hash4( iuv + ivec2(0,0) );
    vec4 ofb = hash4( iuv + ivec2(1,0) );
    vec4 ofc = hash4( iuv + ivec2(0,1) );
    vec4 ofd = hash4( iuv + ivec2(1,1) );
    
    vec2 ddx = dFdx( uv );
    vec2 ddy = dFdy( uv );

    // transform per-tile uvs
    ofa.zw = sign( ofa.zw-0.5 );
    ofb.zw = sign( ofb.zw-0.5 );
    ofc.zw = sign( ofc.zw-0.5 );
    ofd.zw = sign( ofd.zw-0.5 );
    
    // uv's, and derivatives (for correct mipmapping)
    vec2 uva = uv*ofa.zw + ofa.xy, ddxa = ddx*ofa.zw, ddya = ddy*ofa.zw;
    vec2 uvb = uv*ofb.zw + ofb.xy, ddxb = ddx*ofb.zw, ddyb = ddy*ofb.zw;
    vec2 uvc = uv*ofc.zw + ofc.xy, ddxc = ddx*ofc.zw, ddyc = ddy*ofc.zw;
    vec2 uvd = uv*ofd.zw + ofd.xy, ddxd = ddx*ofd.zw, ddyd = ddy*ofd.zw;
        
    // fetch and blend
    vec2 b = smoothstep( 0.25,0.75, fuv );
    
    return mix( mix( textureGrad( samp, uva, ddxa, ddya ), 
                     textureGrad( samp, uvb, ddxb, ddyb ), b.x ), 
                mix( textureGrad( samp, uvc, ddxc, ddyc ),
                     textureGrad( samp, uvd, ddxd, ddyd ), b.x), b.y );
}

void main()
{
	vec3 color = v_Color.rgb;
	vec4 finalColor = vec4(0.0);
	vec4 mask = texture2D(Sampler0,v_Texcoord*1.0/64.0);
	vec4 tex1 = textureNoTile(Sampler1,4.0*v_Texcoord);
	vec4 tex2 = textureNoTile(Sampler2,3.0*v_Texcoord);
	vec4 tex3 = textureNoTile(Sampler3,2.0*v_Texcoord);
	finalColor = mask.r * tex1 + mask.g * tex2 + mask.b * tex3;
	vec4 waterColor = vec4(0.0,0.5,0.8,1.0);
	finalColor = mix(waterColor,finalColor,v_positionY);
	gl_FragColor = finalColor;
	vec3 normal = texture2D(Sampler4, 5.0*v_Texcoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
	vec3 lightDir = normalize(vec3(-1.0,-1.0,-1.0));
	float light = max(0.0,0.5+0.5*dot(-lightDir,normal));
	gl_FragColor.rgb *=light;
	//gl_FragColor.rgb = 0.5+0.5*v_Normal;
}