#version 330 core

in vec2 TexCoords;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;


out vec4 color;

void main()
{    
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);


	
	float visibility = 1.0;


    if(dot(norm, viewDir) < 0.5)
    {
    	color = vec4(1, 0, 0, 1);
    }
    else
    {
    	color = vec4(1, 1, 1, 1);
    }
}
