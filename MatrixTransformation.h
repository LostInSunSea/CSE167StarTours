#pragma once
#include "Group.h"
class MatrixTransformation :
	public Group
{
private:
public:
	double prevTime;
	float ttl = -1;
	glm::mat4 M;
	MatrixTransformation();
	~MatrixTransformation();
	void draw(glm::mat4 C, GLint shader);
	void update(glm::mat4 C);
};

