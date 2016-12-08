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
uniform int shadowMapSize;
uniform int style;


out vec4 color;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

float random(vec3 seed, int i){
    vec4 seed4 = vec4(seed,i);
    float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}

//func decl
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{    
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);


	
	float visibility = 1.0;
    float bias = 0.002;
	vec3 light = CalcDirLight(dirLight, norm, viewDir);

	vec3 DiffuseColor = texture( texture_diffuse1, TexCoords ).rgb;
    

    if(style == 1)
    {
        for(int i = 0; i < 4; i++)
        {
            int index = int(16.0*random(gl_FragCoord.xyy, i))%16;
            visibility -= 0.15*(1-texture(shadowMap, vec3(ShadowCoords.xy + poissonDisk[index]/700.0, (ShadowCoords.z-bias)/ShadowCoords.w).xy).z);

            
        }

    }
    else if (style == 2)
    {
            if(ShadowCoords.x > 1 || ShadowCoords.y > 1 || ShadowCoords.x < 0 ||  ShadowCoords.y < 0 || ShadowCoords.z > 1 ||  ShadowCoords.z < 0)
        {
            visibility = 1.0;
        }
        else if(texture(shadowMap, ShadowCoords.xy).z < ShadowCoords.z - bias)
        {
            visibility = 0.5;
        }
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