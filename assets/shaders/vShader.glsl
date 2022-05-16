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

uniform mat4 toCamera = mat4(1);
uniform mat4 toProjection = mat4(1);
uniform mat4 toWorld = mat4(1);
uniform mat4 modelMatrix = mat4(1);

void main(void)
{
	gl_Position = toProjection * toCamera * toWorld * modelMatrix * vec4(position, 1.0f);

	v.theNormal = normalize(vec3(toWorld * modelMatrix * vec4(normal, 0.0f)));
	v.theTexCoord = texCoord;
	v.thePosition = vec3(toWorld * modelMatrix * vec4(position, 1.0f));


	// For normal mapping
	vec3 T = normalize(vec3(toWorld * modelMatrix * vec4(tangent, 0.0f)));
	vec3 N = normalize(vec3(toWorld * modelMatrix * vec4(normal, 0.0f)));

	vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N));

	v.tangentLightPos = TBN * lightPosition;
	v.tangentCamPos   = TBN * cameraPosition;
	v.tangentPos      = TBN * vec3(toWorld * modelMatrix * vec4(position, 1.0f));
};  
