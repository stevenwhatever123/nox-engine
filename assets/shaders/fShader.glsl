#version 450 core

// From the vertex shader
in Vertex {
    	vec3 thePosition;
		vec2 theTexCoord;
		vec3 theNormal;
		vec3 tangentLightPos;
		vec3 tangentCamPos;
		vec3 tangentPos;
} v;


// The color of the fragment
out vec4 FragmentColor;



// Textures
uniform sampler2D AmbTexture;
uniform sampler2D NormTexture;

void main(void)
{
	vec3 lightSource_diffuse = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightSource_ambient = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightSource_specular = vec3(1.0f, 1.0f, 1.0f);

	vec3 material_ambient = vec3(0.1f, 0.1f, 0.1f);
	vec3 material_diffuse = vec3(0.6f, 0.6f, 0.6f);
	float material_shininess = 0.5f;
	vec3 material_specular = vec3(0.01f, 0.01f, 0.01f);


	vec3 normal = texture(NormTexture, v.theTexCoord).xyz;
	normal = normalize(normal * 2.0 - 1.0);
	
	
  	vec3 lightToFragment = normalize(v.tangentLightPos - v.tangentPos);
  	vec3 cameraToFragment = normalize(v.tangentCamPos - v.tangentPos);


  	// ambient
  	vec3 ambientComp = texture(AmbTexture, v.theTexCoord).xyz * lightSource_ambient;

  	// diffuse color
  	vec3 diffuseComp = material_diffuse * max(dot(normal, lightToFragment), 0.0) * lightSource_diffuse;


  	// specular
  	vec3 halfwayVector = normalize(lightToFragment + cameraToFragment);
  	float spec = pow(max(dot(normal, halfwayVector), 0.0), material_shininess);
  	vec3 specComp = lightSource_specular * (spec * material_specular);


  	vec3 result = ambientComp + diffuseComp + specComp;

	FragmentColor = vec4(result, 1.0);

};
