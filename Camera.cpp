#include "Camera.h"

extern glm::vec3 cam_pos;		// e  | Position of camera
extern glm::vec3 cam_look_at;	// d  | This is where the camera looks at
extern glm::vec3 cam_up;			// up | What orientation "up" is

Camera::Camera()
{
	position = glm::vec3(0.0f, 10.0f, 20.0f);
	look_at = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}


Camera::~Camera()
{
}

void Camera::update(glm::mat4 M)
{
	cam_pos = glm::vec3(M * glm::vec4(position, 1));
	cam_look_at = glm::vec3(M * glm::vec4(look_at, 1));
	cam_up = glm::vec3(M * glm::vec4(up, 1));
}

void Camera::draw(glm::mat4 M, GLint shader)
{
}
