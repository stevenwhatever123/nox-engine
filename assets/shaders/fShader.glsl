#version 450 core

#define NUM_OF_LIGHTS 1


struct LightSource
{
	vec3 tanPos;
};



// From the vertex shader
in Vertex {
    		vec3 thePosition;
		vec2 theTexCoord;
		vec3 theNormal;
		vec3 tangentPos;
} v;



in LightSource lS[NUM_OF_LIGHTS];
in vec3 tanCamPos;


// The color of the fragment
out vec4 FragmentColor;

// Textures
uniform sampler2D AmbTexture;
uniform sampler2D NormTexture;

void main(void)
{

// ========= Light and Material properties ========
	vec3 lightSource_diffuse = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightSource_ambient = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightSource_specular = vec3(1.0f, 1.0f, 1.0f);

	vec3 material_ambient = vec3(0.1f, 0.1f, 0.1f);
	vec3 material_diffuse = vec3(0.6f, 0.6f, 0.6f);
	float material_shininess = 0.5f;
	vec3 material_specular = vec3(0.01f, 0.01f, 0.01f);

// =================================================


// General computations
	vec3 normal = texture(NormTexture, v.theTexCoord).xyz;
	normal = normalize(normal * 2.0 - 1.0);
	
  	vec3 cameraToFragment = normalize(tanCamPos - v.tangentPos);

  	// ambient
  	vec3 ambientComp = texture(AmbTexture, v.theTexCoord).xyz * lightSource_ambient;

	vec3 result = ambientComp;


	// Calculate light for all light sources
	for(int i = 0; i < NUM_OF_LIGHTS; i++)
	{
  		vec3 lightToFragment = normalize(lS[i].tanPos - v.tangentPos);

  		// diffuse color
  		vec3 diffuseComp = material_diffuse * max(dot(normal, lightToFragment), 0.0) * lightSource_diffuse;


  		// specular
  		vec3 halfwayVector = normalize(lightToFragment + cameraToFragment);
  		float spec = pow(max(dot(normal, halfwayVector), 0.0), material_shininess);
  		vec3 specComp = lightSource_specular * (spec * material_specular);

		result += diffuseComp + specComp;

	}


	FragmentColor = vec4(result, 1.0);

};
