#pragma once
#include "Component.h"
#include "MatrixTransformation.h"
class Spawner :
	public Component
{
public:
	MatrixTransformation * spawnee;
	bool spawn;

	Spawner();
	Spawner(MatrixTransformation * spawnee);
	~Spawner();
	void draw(glm::mat4 trans, GLint shader);
	void update(glm::mat4 trans);
};

