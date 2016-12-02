#include "BoundingBox.h"
#include <algorithm>    // std::max
using namespace std;

BoundingBox::BoundingBox()
{
}


BoundingBox::~BoundingBox()
{
}

glm::vec3 BoundingBox::getMaxExtents()
{
	return maxExtents;
}

glm::vec3 BoundingBox::getMinExtents()
{
	return minExtents;
}

void BoundingBox::setMaxExtents(glm::vec3 max)
{
	maxExtents = max;
}

void BoundingBox::setMinExtents(glm::vec3 min)
{
	minExtents = min;
}

bool BoundingBox::aabbTest(BoundingBox other)
{
	glm::vec3 distance1 = other.getMinExtents() - maxExtents;
	glm::vec3 distance2 = other.minExtents - other.getMaxExtents();
	glm::vec3 distance = glm::max(distance1, distance2);
	float maxDistance;
	float temp = max(distance.x, distance.y);
	maxDistance = max(temp, distance.z);
	if (maxDistance < 0) {
		return true;
	}
	else {
		return false;
	}
}

void BoundingBox::update()
{
}

void BoundingBox::draw(glm::mat4, GLint shader)
{
}
