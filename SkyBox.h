#pragma once
#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class SkyBox
{
public:
	//skybox----------------------
	GLuint skyboxVAO, skyboxVBO;
	GLuint cubemapTexture;

	SkyBox();
	~SkyBox();
	void init();
	void drawSkyBox();
	GLuint loadSkyBox(vector<const GLchar*> faces);
};
