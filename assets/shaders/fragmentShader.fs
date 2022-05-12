#version 450 core

in vec3 TextureCoordinate;

out vec4 Color;

uniform samplerCube skybox;

void main()
{    
    Color = texture(skybox, TextureCoordinate);
}
