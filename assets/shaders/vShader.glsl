#version 450 core

#define NUM_OF_LIGHTS 1

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in vec3 tangent;

struct LightSource
{
	vec3 tanPos;
};

out Vertex {
	vec3 thePosition;
	vec2 theTexCoord;
	vec3 theNormal;
	vec3 tangentPos;
} v;

out LightSource lS[NUM_OF_LIGHTS];
out vec3 tanCamPos;

// Uniforms
uniform LightSource lightPosition[NUM_OF_LIGHTS];
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
	vec3 T = vec3(0);//normalize(vec3(0));
	vec3 N = v.theNormal;
	vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N));

	tanCamPos         = TBN * cameraPosition;
	v.tangentPos      = vec3(0);// TBN * vec3(toWorld * modelMatrix * vec4(0, 0, 0, 1.0f));

	// Translate light
	for(int i = 0; i < NUM_OF_LIGHTS; i++)
        	//lS[i].tanPos = vec3(toProjection * toCamera * toWorld * vec4(lightPosition[i].tanPos, 1.0f)); 
        	lS[i].tanPos = TBN * lightPosition[i].tanPos;
	
};  
