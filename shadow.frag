#version 330 core

in vec2 TexCoords;
in vec4 ShadowCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2DShadow shadowMap;

void main()
{    
	
	float visibility = 1.0;
	if(texture(shadowMap, ShadowCoords.xy).z < ShadowCoords.z)
	{
		visibility = 0.5;
	}

    color = vec4(texture(texture_diffuse1, TexCoords));