#include "DirLight.h"



DirLight::DirLight()
{
}

DirLight::DirLight(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->dir = glm::normalize(dir);
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}


DirLight::~DirLight()
{
}
