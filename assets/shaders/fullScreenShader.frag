#version 450 


layout(binding = 0) uniform sampler2D texture1;

out vec4 fragColor;
in vec2 texture_coords;

void main() {

	if(int(gl_FragCoord.x)%2 == 0) 
		fragColor = texture(texture1, texture_coords);
	else
		fragColor = vec4(0, 0, 0, 1);


}
