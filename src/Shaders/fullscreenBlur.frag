#version 130

varying vec2 v_Texcoord;
varying vec3 v_Color;
varying float v_VertexID;
varying vec3 v_Normal;

varying vec2 blurCoordinates[5];

uniform sampler2D AlbedoSampler;
uniform float u_Time;
uniform vec2 u_texelSize;

vec3 GaussianBlur( sampler2D tex0, vec2 centreUV, vec2 pixelOffset )                                                                           
{                                                                                                                                                                    
    vec3 colOut = vec3( 0, 0, 0 );                                                                                                                                   
                                                                                                                                                                     
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////;
    // Kernel width 35 x 35
    //
    const int stepCount = 9;
    //
    const float gWeights[stepCount] ={
       0.10855,
       0.13135,
       0.10406,
       0.07216,
       0.04380,
       0.02328,
       0.01083,
       0.00441,
       0.00157
    };
    const float gOffsets[stepCount] ={
       0.66293,
       2.47904,
       4.46232,
       6.44568,
       8.42917,
       10.41281,
       12.39664,
       14.38070,
       16.36501
    };
                                                                                                                                                                     
    for( int i = 0; i < stepCount; i++ )                                                                                                                             
    {                                                                                                                                                                
        vec2 texCoordOffset = gOffsets[i] * pixelOffset;                                                                                                           
        vec3 col = texture( tex0, centreUV + texCoordOffset ).xyz + texture( tex0, centreUV - texCoordOffset ).xyz;                                                
        colOut += gWeights[i] * col;                                                                                                                               
    }                                                                                                                                                                
                                                                                                                                                                     
    return colOut;                                                                                                                                                   
}       

void main()
{
	vec3 gaussian = GaussianBlur( AlbedoSampler, v_Texcoord, u_texelSize );
	gl_FragColor = vec4(gaussian,1.0);
}




