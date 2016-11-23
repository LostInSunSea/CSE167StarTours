#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "Window.h"
#include <algorithm>    // std::max
struct material_s
{
	float shininess;
	glm::vec3 color;
};
class OBJObject
{
private:
std::vector<unsigned int> indices;
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;


glm::mat4 initToWorld;

float translateX = 0;
float translateY = 0;
float translateZ = 0;

float angle = 0;

float scale = 1;

float orbitAngle = 0;


public:

	OBJObject(const char* filepath, material_s mat);
	~OBJObject();

	void parse(const char* filepath);
	void draw(GLuint shaderProgram);

	void update();

	void spin(float);
	
	void trans(float, float, float);

	void scaleObj(float);

	void orbitObj(float);

	void reset();

	void modPointSize(float);

	int getNumVertex();
	glm::vec3 getVertex(int);
	glm::vec3 getNormal(int);
	glm::mat4 getToWorld();
	
	// These variables are needed for the shader program
	GLuint VBO, VBO2, VAO, EBO;
	GLuint uProjection, uModelview;

	float pointSize = 1;
	glm::mat4 toWorld;

	material_s material;


};

#endif