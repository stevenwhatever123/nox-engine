#version 450 core

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in vec3 tangent;


out Vertex {
	vec3 thePosition;
	vec2 theTexCoord;
	vec3 theNormal;
	vec3 tangentLightPos;
	vec3 tangentCamPos;
	vec3 tangentPos;
} v;



// Uniforms
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform mat4 toCamera;
uniform mat4 toProjection;
uniform mat4 toWorld;

void main(void)
{	
	gl_Position = toProjection * toCamera * toWorld * vec4(position, 1.0f);

	v.theNormal = normal;
	v.theTexCoord = texCoord;
	v.thePosition = position;


	// For normal mapping
	vec3 T = normalize(tangent);
	vec3 N = normalize(normal);
	vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N));

	v.tangentLightPos = TBN * lightPosition;
	v.tangentCamPos   = TBN * cameraPosition;
	v.tangentPos      = TBN * position;
};  
