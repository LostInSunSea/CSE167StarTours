#pragma once
#include "Node.h"
class Component :
	public Node
{
public:
	Component();
	~Component();
	void draw(glm::mat4 trans, GLint shader);
	void update(glm::mat4 trans);
};

