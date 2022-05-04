#version 450 core

in vec3 position;

out vec3 TextureCoordinate;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 Position = projection * view * vec4(position, 1.0);
    gl_Position = Position.xyww;
    TextureCoordinate = position;
}
