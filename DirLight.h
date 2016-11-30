#pragma once
#include "Node.h"
#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


class DirLight:
	public Node
{
public:
	glm::vec3 dir;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	DirLight();
	DirLight(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~DirLight();
};

