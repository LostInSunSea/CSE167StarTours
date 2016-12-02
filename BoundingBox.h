#pragma once
#include "Component.h"
class BoundingBox :
	public Component
{
private:
	//corners
	glm::vec3 minExtents; 
	glm::vec3 maxExtents;

public:
	BoundingBox();
	~BoundingBox();
	glm::vec3 getMaxExtents();
	glm::vec3 getMinExtents();
	void setMaxExtents(glm::vec3 max);
	void setMinExtents(glm::vec3 min);
	bool aabbTest(BoundingBox other);
	void update();
	void draw(glm::mat4, GLint shader);
};

