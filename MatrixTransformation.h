#pragma once
#include "Group.h"
class MatrixTransformation :
	public Group
{
public:
	MatrixTransformation();
	~MatrixTransformation();
	void draw(glm::mat4);
	void update();
};

