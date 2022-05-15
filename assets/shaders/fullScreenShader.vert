#version 450

vec4 points[4] = {
	{-1.0, -1.0, 0.0, 0.0},
	{-1.0,  1.0, 0.0, 1.0},
	{ 1.0,  1.0, 1.0, 1.0},
	{ 1.0, -1.0, 1.0, 0.0},
};


out vec2 texture_coords;

void main() {
	gl_Position   = vec4(points[gl_VertexID].xy, 0, 1);
	texture_coords = points[gl_VertexID].zw;
}
