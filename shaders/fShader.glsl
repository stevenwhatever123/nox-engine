#version 450 core

in vec2 theNormal;
in vec2 theTexCoord;

// The color of the fragment
out vec4 FragmentColor;


uniform vec3 Color;

uniform sampler2D AmbTexture;
uniform sampler2D NormTexture;

void main(void)
{
	
  	FragmentColor = mix(texture(AmbTexture, theTexCoord), texture(NormTexture, theNormal), 0.5);
};
