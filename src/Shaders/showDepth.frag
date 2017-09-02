#version 120

varying vec2 v_Texcoord;
varying vec3 v_Color;
varying float v_VertexID;
varying vec3 v_Normal;
uniform sampler2D AlbedoSampler;
uniform float u_Time;

vec3 fxaa(const in sampler2D tex, const in vec2 pos, const in vec2 texSize) {

    vec2 fxaaQualityRcpFrame = 1.0 / texSize.xy;
    float fxaaQualitySubpix = 0.75;
    float fxaaQualityEdgeThreshold = 0.125;
    float fxaaQualityEdgeThresholdMin = 0.0625;

    vec2 posM = pos.xy;
    vec4 rgbyM = texture2D(tex, posM);
    float lumaM = rgbyM.y;

    // Swizzler Offseter Util
    vec4 sw = vec4(-1.0, 1.0, 1.0, -1.0) * fxaaQualityRcpFrame.xxyy;

    float lumaS = texture2D(tex, posM + vec2(0.0, sw.z)).y;
    float lumaE = texture2D(tex, posM + vec2(sw.y, 0.0)).y;
    float lumaN = texture2D(tex, posM + vec2(0.0, sw.w)).y;
    float lumaW = texture2D(tex, posM + vec2(sw.x, 0.0)).y;

    float maxSM = max(lumaS, lumaM);
    float minSM = min(lumaS, lumaM);

    float maxESM = max(lumaE, maxSM);
    float minESM = min(lumaE, minSM);

    float maxWN = max(lumaN, lumaW);
    float minWN = min(lumaN, lumaW);

    float rangeMax = max(maxWN, maxESM);
    float rangeMin = min(minWN, minESM);

    float rangeMaxScaled = rangeMax * fxaaQualityEdgeThreshold;
    float range = rangeMax - rangeMin;
    float rangeMaxClamped = max(fxaaQualityEdgeThresholdMin, rangeMaxScaled);

    bool earlyExit = range < rangeMaxClamped;
    if(earlyExit)
        return rgbyM.rgb;

    float lumaNW = texture2D(tex, posM + sw.xw).y;
    float lumaSE = texture2D(tex, posM + sw.yz).y;
    float lumaNE = texture2D(tex, posM + sw.yw).y;
    float lumaSW = texture2D(tex, posM + sw.xy).y;

    float lumaNS = lumaN + lumaS;
    float lumaWE = lumaW + lumaE;
    float subpixRcpRange = 1.0/range;
    float subpixNSWE = lumaNS + lumaWE;
    float edgeHorz1 = (-2.0 * lumaM) + lumaNS;
    float edgeVert1 = (-2.0 * lumaM) + lumaWE;

    float lumaNESE = lumaNE + lumaSE;
    float lumaNWNE = lumaNW + lumaNE;
    float edgeHorz2 = (-2.0 * lumaE) + lumaNESE;
    float edgeVert2 = (-2.0 * lumaN) + lumaNWNE;

    float lumaNWSW = lumaNW + lumaSW;
    float lumaSWSE = lumaSW + lumaSE;
    float edgeHorz4 = (abs(edgeHorz1) * 2.0) + abs(edgeHorz2);
    float edgeVert4 = (abs(edgeVert1) * 2.0) + abs(edgeVert2);
    float edgeHorz3 = (-2.0 * lumaW) + lumaNWSW;
    float edgeVert3 = (-2.0 * lumaS) + lumaSWSE;
    float edgeHorz = abs(edgeHorz3) + edgeHorz4;
    float edgeVert = abs(edgeVert3) + edgeVert4;

    float subpixNWSWNESE = lumaNWSW + lumaNESE;
    float lengthSign = fxaaQualityRcpFrame.x;
    bool horzSpan = edgeHorz >= edgeVert;
    float subpixA = subpixNSWE * 2.0 + subpixNWSWNESE;

    if(!horzSpan) lumaN = lumaW;
    if(!horzSpan) lumaS = lumaE;
    if(horzSpan) lengthSign = fxaaQualityRcpFrame.y;
    float subpixB = (subpixA * (1.0/12.0)) - lumaM;

    float gradientN = lumaN - lumaM;
    float gradientS = lumaS - lumaM;
    float lumaNN = lumaN + lumaM;
    float lumaSS = lumaS + lumaM;
    bool pairN = abs(gradientN) >= abs(gradientS);
    float gradient = max(abs(gradientN), abs(gradientS));
    if(pairN) lengthSign = -lengthSign;
    float subpixC = clamp(abs(subpixB) * subpixRcpRange, 0.0, 1.0);

    vec2 posB = posM.xy;
    vec2 offNP;
    offNP.x = (!horzSpan) ? 0.0 : fxaaQualityRcpFrame.x;
    offNP.y = ( horzSpan) ? 0.0 : fxaaQualityRcpFrame.y;
    if(!horzSpan) posB.x += lengthSign * 0.5;
    if( horzSpan) posB.y += lengthSign * 0.5;

    vec2 posN;
    posN.x = posB.x - offNP.x;
    posN.y = posB.y - offNP.y;
    vec2 posP;
    posP.x = posB.x + offNP.x;
    posP.y = posB.y + offNP.y;
    float subpixD = ((-2.0)*subpixC) + 3.0;
    float lumaEndN = texture2D(tex, posN).y;
    float subpixE = subpixC * subpixC;
    float lumaEndP = texture2D(tex, posP).y;

    if(!pairN) lumaNN = lumaSS;
    float gradientScaled = gradient * 1.0/4.0;
    float lumaMM = lumaM - lumaNN * 0.5;
    float subpixF = subpixD * subpixE;
    bool lumaMLTZero = lumaMM < 0.0;

    lumaEndN -= lumaNN * 0.5;
    lumaEndP -= lumaNN * 0.5;
    bool doneN = abs(lumaEndN) >= gradientScaled;
    bool doneP = abs(lumaEndP) >= gradientScaled;
    if(!doneN) posN.x -= offNP.x * 1.5;
    if(!doneN) posN.y -= offNP.y * 1.5;
    bool doneNP = (!doneN) || (!doneP);
    if(!doneP) posP.x += offNP.x * 1.5;
    if(!doneP) posP.y += offNP.y * 1.5;

    if(doneNP) {
        if(!doneN) lumaEndN = texture2D(tex, posN.xy).y;
        if(!doneP) lumaEndP = texture2D(tex, posP.xy).y;
        if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
        if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
        doneN = abs(lumaEndN) >= gradientScaled;
        doneP = abs(lumaEndP) >= gradientScaled;
        if(!doneN) posN.x -= offNP.x * 2.0;
        if(!doneN) posN.y -= offNP.y * 2.0;
        doneNP = (!doneN) || (!doneP);
        if(!doneP) posP.x += offNP.x * 2.0;
        if(!doneP) posP.y += offNP.y * 2.0;

        if(doneNP) {
            if(!doneN) lumaEndN = texture2D(tex, posN.xy).y;
            if(!doneP) lumaEndP = texture2D(tex, posP.xy).y;
            if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
            if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
            doneN = abs(lumaEndN) >= gradientScaled;
            doneP = abs(lumaEndP) >= gradientScaled;
            if(!doneN) posN.x -= offNP.x * 4.0;
            if(!doneN) posN.y -= offNP.y * 4.0;
            doneNP = (!doneN) || (!doneP);
            if(!doneP) posP.x += offNP.x * 4.0;
            if(!doneP) posP.y += offNP.y * 4.0;

            if(doneNP) {
                if(!doneN) lumaEndN = texture2D(tex, posN.xy).y;
                if(!doneP) lumaEndP = texture2D(tex, posP.xy).y;
                if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
                if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
                doneN = abs(lumaEndN) >= gradientScaled;
                doneP = abs(lumaEndP) >= gradientScaled;
                if(!doneN) posN.x -= offNP.x * 12.0;
                if(!doneN) posN.y -= offNP.y * 12.0;
                doneNP = (!doneN) || (!doneP);
                if(!doneP) posP.x += offNP.x * 12.0;
                if(!doneP) posP.y += offNP.y * 12.0;

            }

        }

    }

    float dstN = posM.x - posN.x;
    float dstP = posP.x - posM.x;
    if(!horzSpan) dstN = posM.y - posN.y;
    if(!horzSpan) dstP = posP.y - posM.y;

    bool goodSpanN = (lumaEndN < 0.0) != lumaMLTZero;
    float spanLength = (dstP + dstN);
    bool goodSpanP = (lumaEndP < 0.0) != lumaMLTZero;
    float spanLengthRcp = 1.0/spanLength;

    bool directionN = dstN < dstP;
    float dst = min(dstN, dstP);
    bool goodSpan = directionN ? goodSpanN : goodSpanP;
    float subpixG = subpixF * subpixF;
    float pixelOffset = (dst * (-spanLengthRcp)) + 0.5;
    float subpixH = subpixG * fxaaQualitySubpix;

    float pixelOffsetGood = goodSpan ? pixelOffset : 0.0;
    float pixelOffsetSubpix = max(pixelOffsetGood, subpixH);
    if(!horzSpan) posM.x += pixelOffsetSubpix * lengthSign;
    if( horzSpan) posM.y += pixelOffsetSubpix * lengthSign;

    return texture2D(tex, posM).xyz;
}

float linearDepth(float depthSample)
{
	float zNear = 0.1;
	float zFar = 1000.0;
    return (2.0 * zNear) / (zFar + zNear - depthSample * (zFar - zNear));
}

vec3 Saturation(vec3 rgb, float adjustment)
{
    // Algorithm from Chapter 16 of OpenGL Shading Language
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);
    vec3 intensity = vec3(dot(rgb, W));
    return mix(intensity, rgb, adjustment);
}

vec3 ChromaticAbberation(sampler2D sampler,vec2 texCoord, float amount, vec2 invTexSize)
{
	vec2 uv = texCoord - 0.5;
	vec2 gUV = uv * 1.01 * amount;
	vec2 bUV = uv * 1.02 * amount;
	vec2 N = invTexSize*vec2(0.0,1.0); 
	vec2 W = invTexSize*vec2(-1.0,0.0); 
	vec2 E = invTexSize*vec2(1.0,0.0); 
	vec2 S = invTexSize*vec2(0.0,-1.0); 
	float r = texture2D(sampler, texCoord).r;
	float g = texture2D(sampler, gUV+0.5+N).g + texture2D(sampler, gUV+0.5+S).g + texture2D(sampler, gUV+0.5+W).g+ texture2D(sampler, gUV+0.5+E).g;
	float b = texture2D(sampler, bUV+0.5+N).b + texture2D(sampler, bUV+0.5+S).b + texture2D(sampler, bUV+0.5+W).b+ texture2D(sampler, bUV+0.5+E).b;
	return vec3(r,0.25*g,0.25*b);
}

vec3 Contrast(sampler2D sampler,vec2 texCoord, float amount)
{
	vec3 color = texture2D(sampler, texCoord).rgb;
	color = color-0.5;
	color = amount*color+0.5;
	return color;
}

vec3 FilmGrain(vec2 texCoord, float noiseStrength)
{
	float x = (texCoord.x + 4) * (texCoord.y + 4) * (123 * 10);
    vec3 grain = vec3(mod((mod(x, 13.0) + 1.0) * (mod(x, 123.0) + 1), 0.01) - 0.005) * noiseStrength;
	return grain;
}

float Vignette(vec2 texCoord, float strength)
{
	vec2 uv = texCoord - 0.5;
	uv = abs(uv);
	return min(1.0,1.5-strength*length(uv*2.0));
}

void main()
{
	vec4 color = vec4(v_Color.rgb,1.0);
	vec4 diffuse = color*vec4(fxaa(AlbedoSampler,v_Texcoord,vec2(1280.0,720.0)),1.0);
	//diffuse.rgb = ChromaticAbberation(AlbedoSampler,v_Texcoord,1.01,1.0/vec2(1280.0,720.0));
	//diffuse.rgb = Saturation(diffuse.rgb,1.5);
	//diffuse.rgb = Contrast(AlbedoSampler,v_Texcoord,1.5) + FilmGrain(v_Texcoord,2.0);
	//diffuse.rgb *= Vignette(v_Texcoord,1.0);
	//diffuse = vec4(vec3(linearDepth(texture2D(AlbedoSampler,v_Texcoord).x)),1.0);
	gl_FragColor = diffuse;

}