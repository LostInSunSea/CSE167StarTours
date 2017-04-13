#pragma once

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Node
{
public:
	bool toDelete = false;
	Node();
	~Node();
	virtual void update(glm::mat4 trans)=0;
	virtual void draw(glm::mat4 trans, GLint shader)=0;
};

