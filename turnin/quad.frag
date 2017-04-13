#version 330 core

in vec2 TexCoords;

layout(location = 0) out vec3 color;

uniform sampler2D quadtexture;

void main()
{    
    color = texture(quadtexture, TexCoords).xyz;
}