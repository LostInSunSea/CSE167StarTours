#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "OBJObject.h"
#include "DirLight.h"
#include "AudioEngine.h"
#include "SkyBox.h"

struct dirLight_s
{
	glm::vec3 dir;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct pointLight_s
{
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};



struct spotlight_s
{
	glm::vec3 position;
	glm::vec3 direction;

	float angle;
	float exponent;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	
};


class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void checkAllBoundingBoxes();

	static glm::vec3 lastBallPoint;

	static double prevx, prevy;

	static dirLight_s dirLight;
	static pointLight_s pointLight;
	static spotlight_s spotLight;

	static int manip;
	static int light;
	static bool normal;
};

#endif
