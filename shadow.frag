#version 330 core

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};



in vec2 TexCoords;
in vec4 ShadowCoords;

in vec3 Normal;
in vec3 FragPos;


uniform vec3 viewPos;
uniform DirLight dirLight;

uniform sampler2D texture_diffuse1;
uniform sampler2D shadowMap;


out vec4 color;

//func decl
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{    
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);


	
	float visibility = 1.0;

	vec3 light = CalcDirLight(dirLight, norm, viewDir);

	vec3 DiffuseColor = texture( texture_diffuse1, TexCoords ).rgb;

	if(texture(shadowMap, ShadowCoords.xy).z < ShadowCoords.z - 0.01)
	{
		visibility = 0.5;
	}

    color = vec4(visibility * DiffuseColor, 1) * vec4(light, 1);
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 5.0);//material.shininess);
    // Combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;
    return (ambient + diffuse + specular);
}