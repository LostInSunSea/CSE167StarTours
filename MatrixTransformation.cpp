#include "MatrixTransformation.h"



MatrixTransformation::MatrixTransformation()
{
	M = glm::mat4(1.0f);
}


MatrixTransformation::~MatrixTransformation()
{
}

void MatrixTransformation::draw(glm::mat4 C, GLint shader)
{
	glm::mat4 transform = C * M;
	for (Node * n : allNodes)
	{
		n->draw(transform, shader);
	}
}

void MatrixTransformation::update(glm::mat4 C)
{
	for (Node * n : allNodes)
	{
		n->update(C * M);
	}
}
