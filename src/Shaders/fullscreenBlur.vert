
//http://www.sunsetlakesoftware.com/2013/10/21/optimizing-gaussian-blurs-mobile-gpu

#version 130
varying vec2 v_Texcoord;
varying vec3 v_Color;
varying float v_VertexID;

uniform vec2 u_texelSize;

const vec2 positions[4] = vec2[](
        vec2(-1, -1),
        vec2(+1, -1),
        vec2(-1, +1),
        vec2(+1, +1)
    );
    const vec2 coords[4] = vec2[](
        vec2(0, 0),
        vec2(1, 0),
        vec2(0, 1),
        vec2(1, 1)
    );

void main( ) {
    v_Texcoord = coords[gl_VertexID];
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
	v_Color = vec3(0.2*gl_VertexID);
	v_VertexID = gl_VertexID;
}