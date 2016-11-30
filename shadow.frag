#version 330 core

in vec2 TexCoords;
in vec4 ShadowCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D shadowMap;

void main()
{    
	
	float visibility = 1.0;

	vec3 DiffuseColor = texture( texture_diffuse1, TexCoords ).rgb;

	if(texture(shadowMap, ShadowCoords.xy).z < ShadowCoords.z - 0.01)
	{
		visibility = 0.5;
	}

    color = visibility * vec4(DiffuseColor, 1);
}