#pragma once
#include "Component.h"
class BoundingBox :
	public Component
{
private:
	//corners
	glm::vec3 minExtents; 
	glm::vec3 maxExtents;
	// Define the coordinates and indices needed to draw the cube. Note that it is not necessary
	// to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
	// This just looks nicer since it's easy to tell what coordinates/indices belong where.
	const GLfloat vertices[8][3] = {
		// "Front" vertices
		{ -2.0, -2.0,  2.0 },{ 2.0, -2.0,  2.0 },{ 2.0,  2.0,  2.0 },{ -2.0,  2.0,  2.0 },
		// "Back" vertices
		{ -2.0, -2.0, -2.0 },{ 2.0, -2.0, -2.0 },{ 2.0,  2.0, -2.0 },{ -2.0,  2.0, -2.0 }
	};

	// Note that GL_QUADS is deprecated in modern OpenGL (and removed from OSX systems).
	// This is why we need to draw each face as 2 triangles instead of 1 quadrilateral
	const GLuint indices[6][6] = {
		// Front face
		{ 0, 1, 2, 2, 3, 0 },
		// Top face
		{ 1, 5, 6, 6, 2, 1 },
		// Back face
		{ 7, 6, 5, 5, 4, 7 },
		// Bottom face
		{ 4, 0, 3, 3, 7, 4 },
		// Left face
		{ 4, 5, 1, 1, 0, 4 },
		// Right face
		{ 3, 2, 6, 6, 7, 3 }
	};
	float maxX;
	float maxY;
	float maxZ;
	float minX;
	float minY;
	float minZ;

public:
	BoundingBox();
	~BoundingBox();
	glm::vec3 getMaxExtents();
	glm::vec3 getMinExtents();
	void setMaxExtents(glm::vec3 max);
	void setMinExtents(glm::vec3 min);
	bool aabbTest(BoundingBox other);
	void update();
	void draw(glm::mat4, GLint shader);
	void init();
	//variables
	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
	glm::mat4 toWorld;
};

