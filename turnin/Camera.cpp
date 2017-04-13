#include "Camera.h"
#include <glm/gtx/matrix_decompose.hpp>

extern glm::vec3 cam_pos;		// e  | Position of camera
extern glm::vec3 cam_look_at;	// d  | This is where the camera looks at
extern glm::vec3 cam_up;			// up | What orientation "up" is

Camera::Camera()
{
	position = glm::vec3(0.0f, 5.0f, 20.0f);
	look_at = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}


Camera::~Camera()
{
}

void Camera::update(glm::mat4 M)
{
	/*
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(M, scale, rotation, translation, skew, perspective);
	*/

	cam_pos = glm::vec3(M * glm::vec4(position, 1));
	cam_look_at = glm::vec3(M * glm::vec4(look_at, 1));
	M[3][0] = 0;
	M[3][1] = 0;
	M[3][2] = 0;
	cam_up = glm::normalize(glm::vec3(M * glm::vec4(up, 1)));
	/*
	glm::vec3 cross;
	glm::vec3 side;
	glm::vec3 upVector;

	cross = glm::cross(cam_pos, cam_look_at);
	side = glm::normalize(cross);
	cam_up = glm::normalize(glm::cross(side, glm::vec3(0, 0, -1)));
	
	glm::vec3 proj = glm::dot(cam_pos, cam_look_at) * cam_pos;
	proj = glm::normalize(proj);
	cam_up = proj;
	*/
}

void Camera::draw(glm::mat4 M, GLint shader)
{
}
