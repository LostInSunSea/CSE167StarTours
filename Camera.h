#pragma once
#include "Node.h"
class Camera :
	public Node
{
public:
	
	glm::vec3 position;
	glm::vec3 look_at;
	glm::vec3 up;
	Camera();
	~Camera();

	void update(glm::mat4 M);
	void draw(glm::mat4 M, GLint shader);
};

