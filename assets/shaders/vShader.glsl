#version 450 core

in vec3 position;
in vec2 normal;
in vec2 texCoord;

out vec2 theNormal;
out vec2 theTexCoord;

uniform mat4 toCamera;
uniform mat4 toProjection;

void main(void)
{	
	gl_Position = toProjection * toCamera * vec4(position, 1.0f);
	theNormal = normal;
	theTexCoord = texCoord;
};
