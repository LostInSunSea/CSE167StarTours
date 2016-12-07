#include "Spawner.h"

extern MatrixTransformation * laserWorld;

Spawner::Spawner()
{
}
Spawner::Spawner(MatrixTransformation * spawnee)
{
	this->spawnee = spawnee;
}

Spawner::~Spawner()
{
}

void Spawner::draw(glm::mat4 trans, GLint shader)
{
	//nothing to draw
}
void Spawner::update(glm::mat4 trans)
{
	if (spawn)
	{
		std::cerr << "SHOOT" << std::endl;
		//spawn = false;
		MatrixTransformation * MT = new MatrixTransformation();
		MT->M = trans;
		MT->addChild(spawnee);
		laserWorld->addChild(MT);
	}
}
