#include "OBJObject.h"
using namespace std;




OBJObject::OBJObject(const char *filepath, material_s mat) 
{
	material = mat;
	//material.color = glm::vec3(1.0f, 0.8f, 0.0f);
	//material.shininess = 0.5f;
	toWorld = glm::mat4(1.0f);
	
	parse(filepath);

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), (GLfloat *)&vertices[0][0], GL_STATIC_DRAW);
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

	//NORMALS
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), (GLfloat *)&normals[0][0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

					 // We've sent the vertex data over to OpenGL, but there's still something missing.
					 // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}
OBJObject::~OBJObject()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
}

void OBJObject::parse(const char *filepath) 
{
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	FILE * fp;
	float x, y, z;
	float r, g, b;
	string buf;
	int c1, c2;
	char buffer[BUFSIZ];
	float minx;
	float maxx;
	float miny;
	float maxy;
	float minz;
	float maxz;

	minx = miny = minz = std::numeric_limits<float>::infinity();
	maxx = maxy = maxz = -std::numeric_limits<float>::infinity();
	
	fp = fopen(filepath, "rb");
	if (fp == NULL) { cerr << "error loading file" << endl; exit(-1); }  // just in case the file can't be found or is corrupt

	while ((c1 = fgetc(fp)) != EOF)
	{
		if (c1 == '#')
		{
			fgets(buffer, BUFSIZ, fp);
		}
		else
		{
			c2 = fgetc(fp);
			if (c1 == 'v' && c2 == ' ')
			{
				fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
				vertices.push_back(glm::vec3(x, y, z));

				if (x < minx)
				{
					minx = x;
				}
				if (x > maxx)
				{
					maxx = x;
				}
				if (y < miny)
				{
					miny = y;
				}
				if (y > maxy)
				{
					maxy = y;
				}
				if (z < minz)
				{
					minz = z;
				}
				if (z > maxz)
				{
					maxz = z;
				}
				//cerr << x << " " << y << " " << z << endl;
			}
			else if (c1 == 'v' && c2 == 'n')
			{
				fscanf(fp, "%f %f %f", &x, &y, &z);
				normals.push_back(glm::vec3(x, y, z));

				//cerr << x << " " << y << " " << z << endl;
			}
			else if (c1 == 'f' && c2 == ' ')
			{
				unsigned int fv1, fn1, fv2, fn2, fv3, fn3;
				fscanf(fp, "%d//%d %d//%d %d//%d", &fv1, &fn1, &fv2, &fn2, &fv3, &fn3);
				if (fv1 != fn1 || fv2 != fn2 || fv3 != fn3)
				{
					cerr << "error loading faces" << endl;
					exit(-2);
				}
				indices.push_back(fv1 - 1);
				indices.push_back(fv2 - 1);
				indices.push_back(fv3 - 1);
				//cerr << fv1 << " " << fv2 << " " << fv3 << " " << endl;
			}
		}


		

	}
	cerr << vertices.size() << " " << normals.size() << " " << indices.size() << endl;
	cerr << "mins " << minx << " " << miny << " " << minz << endl;
	cerr << "maxs " << maxx << " " << maxy << " " << maxz << endl;


	float avgx = minx / 2 + maxx / 2;
	float avgy = miny / 2 + maxy / 2;
	float avgz = minz / 2 + maxz / 2;

	cerr << "avgs " << avgx << " " << avgy << " " << avgz << endl;
	
	toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(-avgx, -avgy, -avgz)) * toWorld;

	float initScale = std::max(std::abs(maxx - avgx), std::max(std::abs(maxy - avgy), std::max(std::abs(maxz - avgz), std::max(std::abs(minx - avgx) , std::max(std::abs(miny - avgy) , std::abs(minz - avgz) )))));
	
	
	if (initScale < 0)
	{
		initScale = -initScale;
	}
	cerr << "scale " << initScale <<endl;

	initScale = 1.0f / initScale;
	toWorld = glm::scale(glm::mat4(1.0f) , glm::vec3(initScale, initScale, initScale)) * toWorld;
	initToWorld = toWorld;
	

	fclose(fp);
	


}

void OBJObject::draw(GLuint shaderProgram)
{
	
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	glm::mat3 normal;

	glm::vec4 pointLight_c = Window::V * glm::vec4(Window::pointLight.position, 1);
	glm::vec4 spotLight_c = Window::V * glm::vec4(Window::spotLight.position, 1);



	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

	int lightToPass;
	if (Window::normal)
	{
		lightToPass = 0;
	}
	else
	{
		lightToPass = Window::light;
	}

	glUniform1i(glGetUniformLocation(shaderProgram, "light"), lightToPass);
	glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), 0, 0, 20.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), material.shininess);
	glUniform3f(glGetUniformLocation(shaderProgram, "material.color"), material.color.x, material.color.y, material.color.z);

	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), Window::dirLight.dir.x, Window::dirLight.dir.y, Window::dirLight.dir.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), Window::dirLight.ambient.x, Window::dirLight.ambient.y, Window::dirLight.ambient.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), Window::dirLight.diffuse.x, Window::dirLight.diffuse.y, Window::dirLight.diffuse.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), Window::dirLight.specular.x, Window::dirLight.specular.y, Window::dirLight.specular.z);
	
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.position"), pointLight_c.x, pointLight_c.y, pointLight_c.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.ambient"), Window::pointLight.ambient.x, Window::pointLight.ambient.y, Window::pointLight.ambient.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.diffuse"), Window::pointLight.diffuse.x, Window::pointLight.diffuse.y, Window::pointLight.diffuse.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.specular"), Window::pointLight.specular.x, Window::pointLight.specular.y, Window::pointLight.specular.z);

	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.direction"), Window::spotLight.direction.x, Window::spotLight.direction.y, Window::spotLight.direction.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.position"), spotLight_c.x, spotLight_c.y, spotLight_c.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.ambient"), Window::spotLight.ambient.x, Window::spotLight.ambient.y, Window::spotLight.ambient.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.diffuse"), Window::spotLight.diffuse.x, Window::spotLight.diffuse.y, Window::spotLight.diffuse.z);
	glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.specular"), Window::spotLight.specular.x, Window::spotLight.specular.y, Window::spotLight.specular.z);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.angle"), Window::spotLight.angle);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.exponent"), Window::spotLight.exponent);
	




	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using all indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, sizeof(unsigned int) * indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
	
}

void OBJObject::update()
{
	//spin(0.025f);

	
}

void OBJObject::spin(float deg)
{
	if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
	this->angle += deg;
	this->toWorld = glm::rotate(this->toWorld, deg, glm::vec3(0.0f, 1.0f, 0.0f));
}

void OBJObject::trans(float magx, float magy, float magz)
{
	glm::mat4 transM = glm::translate(glm::mat4(1.0f), glm::vec3(magx, magy, magz));
	this->toWorld = transM * this->toWorld;
	translateX += magx;
	translateY += magy;
	translateY += magz;
}
void OBJObject::scaleObj(float mag)
{
	this->toWorld = glm::scale(this->toWorld, glm::vec3(mag, mag, mag));
	scale += mag;
}
void OBJObject::orbitObj(float mag)
{

	if (this->orbitAngle > 360.0f || this->orbitAngle < -360.0f) this->orbitAngle = 0.0f;
	this->orbitAngle += mag;
	glm::mat4 orbitM = glm::rotate(glm::mat4(1.0f), mag, glm::vec3(0.0f, 0.0f, 1.0f));
	this->toWorld = orbitM * this->toWorld;
}
void OBJObject::modPointSize(float mag)
{
	this->pointSize += mag;
	if (this->pointSize < 1)
	{
		this->pointSize = 1;
	}
	glPointSize(this->pointSize);
}

void OBJObject::reset()
{
	this->toWorld = initToWorld;
	this->pointSize = 1;
}

glm::vec3 OBJObject::getVertex(int i)
{
	return this->vertices[i];
}
glm::vec3 OBJObject::getNormal(int i)
{
	return this->normals[i];
}

glm::mat4 OBJObject::getToWorld()
{
	return this->toWorld;
}

int OBJObject::getNumVertex()
{
	return vertices.size();
}