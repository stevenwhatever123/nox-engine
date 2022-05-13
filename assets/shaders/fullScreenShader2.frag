#version 450 


layout(binding = 0) uniform sampler2D texture1;

out vec4 fragColor;
in vec2 texture_coords;
uniform float dt;

void main() {

	if(int(gl_FragCoord.y)%4 == 0)
		fragColor = texture(texture1, vec2(texture_coords.x, texture_coords.y + sin((dt))));
	else
		fragColor = vec4(0, 0.1, 0, 1);


}
