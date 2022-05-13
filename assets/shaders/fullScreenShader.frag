#version 450 


layout(binding = 0) uniform sampler2D texture1;

out vec4 fragColor;
in vec2 texture_coords;
uniform float dt;

float n = 0.1;
float f = 1000.0f;

void main() {

	float z_val = texture(texture1, texture_coords).w*2.0 - 1.0 ;
	z_val = (2.0*n*f)/(f+n - z_val*(f - n));

	fragColor = vec4(vec3(z_val)/f, 1.0);
}
