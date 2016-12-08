#pragma once
#include "Component.h"
class BoundingBox :
	public Component
{
private:
	//corners
	glm::vec3 minExtents=glm::vec3(-0.5,-0.5,-0.5); 
	glm::vec3 maxExtents=glm::vec3(0.5,0.5,0.5);
	glm::mat4 matrixTrans = glm::mat4(1);
	// Define the coordinates and indices needed to draw the cube. Note that it is not necessary
	// to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
	// This just looks nicer since it's easy to tell what coordinates/indices belong where.
	// Define the coordinates and indices needed to draw the cube. Note that it is not necessary
	// to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
	// This just looks nicer since it's easy to tell what coordinates/indices belong where.
	const GLfloat vertices[20][3] = {
		// "Front" vertices a,b,c,d
		{ -0.5, -0.5,  0.5 },{ 0.5, -0.5,  0.5 },{ 0.5,  0.5,  0.5 },{ -0.5,  0.5,  0.5 },
		// "Back" vertices e,f,g,h
		{ -0.5, -0.5, -0.5 },{ 0.5, -0.5, -0.5 },{ 0.5,  0.5, -0.5 },{ -0.5,  0.5, -0.5 },
		//a e b f 
		{ -0.5, -0.5,  0.5 },{ -0.5, -0.5, -0.5 },{ 0.5, -0.5,  0.5 },{ 0.5, -0.5, -0.5 },
		//c g d h
		{ 0.5,  0.5,  0.5 },{ 0.5,  0.5, -0.5 },{ -0.5,  0.5,  0.5 },{ -0.5,  0.5, -0.5 },
		//a d e h
		{ -0.5, -0.5,  0.5 },{ -0.5,  0.5,  0.5 },{ -0.5, -0.5, -0.5 },{ -0.5,  0.5, -0.5 }
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
	bool nonstatic=true;
	float height;
	float width;
	float length;
	int color=2;
	bool draws=true;
public:
	BoundingBox();
	~BoundingBox();
	glm::vec3 getMaxExtents();
	glm::vec3 getMinExtents();
	glm::mat4 getTransMatrix();
	void setMaxExtents(glm::vec3 max);
	void setMinExtents(glm::vec3 min);
	bool aabbTest(BoundingBox * other);
	void update(glm::mat4 trans);
	void draw(glm::mat4, GLint shader);
	void setNonStatic(bool position);
	void init();
	void setHeight(float heightz);
	void setWidth(float widthz);
	void setLength(float lengthz);
	void setColor(int colorz);
	void setDraw(bool drawz);
	//variables
	//These variables are needed for the shader program
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
	glm::mat4 toWorld;
};

