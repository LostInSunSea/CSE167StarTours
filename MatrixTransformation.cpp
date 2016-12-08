#include "MatrixTransformation.h"
#include <ctime>


MatrixTransformation::MatrixTransformation()
{
	M = glm::mat4(1.0f);
	prevTime = std::clock();
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
		if (n->toDelete == true)
		{
			std::cerr << "deleted" << std::endl;
			removeChild(n);
			delete n;
		}
	}
	if (ttl > 0)
	{
		double duration = (std::clock() - prevTime) / (double)CLOCKS_PER_SEC;
		ttl -= duration;
		prevTime = std::clock();
		if (ttl <= 0)
		{
			std::cerr << "delet this" << std::endl;
			toDelete = true;
		}
	}
}
