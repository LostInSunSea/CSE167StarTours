#include "BoundingBox.h"
#include <algorithm>    // std::max
#include "Window.h"
using namespace std;

BoundingBox::BoundingBox()
{
	init();
}


BoundingBox::~BoundingBox()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

glm::vec3 BoundingBox::getMaxExtents()
{
	return maxExtents;
}

glm::vec3 BoundingBox::getMinExtents()
{
	return minExtents;
}

glm::mat4 BoundingBox::getTransMatrix()
{
	return matrixTrans;
}

void BoundingBox::setMaxExtents(glm::vec3 max)
{
	maxExtents = max;
}

void BoundingBox::setMinExtents(glm::vec3 min)
{
	minExtents = min;
}

bool BoundingBox::aabbTest(BoundingBox* other)
{
	glm::vec4 thisMinExtents = matrixTrans * glm::vec4(minExtents,1.0f);
	glm::vec4 thisMaxExtents = matrixTrans * glm::vec4(maxExtents, 1.0f);
	glm::vec4 othrMinExtents = other->getTransMatrix() * glm::vec4(other->getMinExtents(), 1.0f);
	glm::vec4 othrMaxExtents = other->getTransMatrix() * glm::vec4(other->getMaxExtents(), 1.0f);

	glm::vec3 distance1 = glm::vec3(othrMinExtents - thisMaxExtents);
	glm::vec3 distance2 = glm::vec3(thisMinExtents - othrMaxExtents);
	glm::vec3 distance = glm::max(distance1, distance2);
	
	if (other->getTransMatrix() == matrixTrans) {
		cout << "hello world";
	}

	cout << "-----------------------------------" << endl;
	cout << "distance 1: "<< endl;
	cout << "x: " << distance1.x << endl;
	cout << "y: " << distance1.y << endl;
	cout << "z: " << distance1.z << endl;
	cout << "distance 2: " << endl;
	cout << "x: " << distance2.x << endl;
	cout << "y: " << distance2.y << endl;
	cout << "z: " << distance2.z << endl;

	cout << "this min: " << endl;
	cout << "x: " << thisMinExtents.x << endl;
	cout << "y: " << thisMinExtents.y << endl;
	cout << "z: " << thisMinExtents.z << endl;
	cout << "this max: " << endl;
	cout << "x: " << thisMaxExtents.x << endl;
	cout << "y: " << thisMaxExtents.y << endl;
	cout << "z: " << thisMaxExtents.z << endl;
	cout << "othr min: " << endl;
	cout << "x: " << othrMinExtents.x << endl;
	cout << "y: " << othrMinExtents.y << endl;
	cout << "z: " << othrMinExtents.z << endl;
	cout << "othr max: " << endl;
	cout << "x: " << othrMaxExtents.x << endl;
	cout << "y: " << othrMaxExtents.y << endl;
	cout << "z: " << othrMaxExtents.z << endl;

	float maxDistance;
	float temp = max(distance.x, distance.y);
	maxDistance = max(temp, distance.z);
	cout << "distance:" << maxDistance << endl;
	if (maxDistance < 0) {
		cout << "intersect: 1" << endl;
		cout << "-----------------------------------"<<endl;
		return true;
	}
	else {
		cout << "intersect: 0" << endl;
		cout << "-----------------------------------" << endl;

		return false;
	}
}

void BoundingBox::update(glm::mat4 trans)
{
	matrixTrans=trans;
}

void BoundingBox::draw(glm::mat4 trans, GLint shaderProgram)
{
	GLint shaderProg = 3;
	glUseProgram(shaderProg);
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview;
	//nonstatic = true;
	if (nonstatic) {
		modelview = Window::V * trans;
	}
	else {
		glm::mat4 temp = trans;
		temp[0][0] = 1;
		temp[0][1] = 0;
		temp[0][2] = 0;
		temp[1][0] = 0;
		temp[1][1] = 1;
		temp[1][2] = 0;
		temp[2][0] = 0;
		temp[2][1] = 0;
		temp[2][2] = 1;
		modelview=Window::V*temp*glm::scale(glm::mat4(1.0f), glm::vec3(width,height, length));
	}
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProg, "projection");
	uModelview = glGetUniformLocation(shaderProg, "modelview");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

	glUniform3f(glGetUniformLocation(shaderProg, "viewPos"), 0, 0, 20.0f);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glLineWidth(4);
	glDrawArrays(GL_LINE_STRIP, 0, 12 * 3);
	glBindVertexArray(0);
}

void BoundingBox::setNonStatic(bool position)
{
	nonstatic = position;
}

void BoundingBox::init()
{
	toWorld = glm::mat4(1.0f);

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

					 // We've sent the vertex data over to OpenGL, but there's still something missing.
					 // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}

void BoundingBox::setHeight(float heightz)
{
	height = heightz;
}

void BoundingBox::setWidth(float widthz)
{
	width = widthz;
}

void BoundingBox::setLength(float lengthz)
{
	length = lengthz;
}
