#include "window.h"
#include "Model.h"
#include <glm/gtc/type_ptr.hpp>

const char* window_title = "Galaxy Battles: Episode V - The Imperium's Counter Attack";
Cube * cube;
GLint shaderProgram;
GLint testShader;
GLint depthShader;
GLint shadowShader;

/*
OBJObject * current;
OBJObject * bunny;
OBJObject * dragon;
OBJObject * bear;
*/

Model * testModel;
AudioEngine * audioEngine=new AudioEngine();


// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"
#define TEST_VERTEX_SHADER "../test.vert"
#define TEST_FRAGMENT_SHADER "../test.frag"
#define DEPTH_VERTEX_SHADER "../depth.vert"
#define DEPTH_FRAGMENT_SHADER "../depth.frag"
#define SHADOW_VERTEX_SHADER "../shadow.vert"
#define SHADOW_FRAGMENT_SHADER "../shadow.frag"



// Default camera parameters
glm::vec3 cam_pos(0.0f, 10.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

dirLight_s Window::dirLight;
pointLight_s Window::pointLight;
spotlight_s Window::spotLight;


int Window::manip = 0;
int Window::light = 1;
bool Window::normal = false;

GLuint DepthFrameBuffer = 0;
GLuint depthTexture;

DirLight * sun;


void Window::initialize_objects()
{

	audioEngine->init(cam_pos);
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	testShader = LoadShaders(TEST_VERTEX_SHADER, TEST_FRAGMENT_SHADER);
	depthShader = LoadShaders(DEPTH_VERTEX_SHADER, DEPTH_FRAGMENT_SHADER);
	shadowShader = LoadShaders(SHADOW_VERTEX_SHADER, SHADOW_FRAGMENT_SHADER);

	//testModel = new Model("../Assets/Models/nanosuit/nanosuit.obj");
	//testModel = new Model("../Assets/Models/snowspeeder/Star Wars Snowspeeder.obj");
	//testModel = new Model("../Assets/Models/grass/grassCube.obj");
	testModel = new Model("../Assets/Models/scene/scene.obj");


	sun = new DirLight(glm::vec3(-1, -3, 0), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.3f, 0.3f, 0.3f));

	//set up depth texture
	glGenFramebuffers(1, &DepthFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, DepthFrameBuffer);

	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

						   // Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		exit(1);
	}
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	//update
}

void Window::display_callback(GLFWwindow* window)
{
	//testing
	glm::mat4 model;
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// It's a bit too big for our scene, so scale it down
	//Draw depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, DepthFrameBuffer);
	glViewport(0, 0, 1024, 1024);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Cull back-facing triangles

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use depth shader
	glUseProgram(depthShader);

	glm::vec3 lightInvDir = -(sun->dir);

	//need to fine tune/gen automatically
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));


	//BASED OFF EACH OBJECT
	glm::mat4 depthModelMatrix = model;
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(glGetUniformLocation(depthShader, "depthMVP"), 1, GL_FALSE, &depthMVP[0][0]);

	testModel->Draw(depthShader);
	
	

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	glm::mat4 depthBiasMVP = biasMatrix*depthMVP;

	

	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Window::resize_callback(window, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles


	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glUseProgram(shadowShader);
	glm::mat4 projection = Window::P;
	glm::mat4 view = Window::V;
	glUniformMatrix4fv(glGetUniformLocation(shadowShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shadowShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shadowShader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shadowShader, "DepthBiasMVP"), 1, GL_FALSE, &depthBiasMVP[0][0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glUniform1i(glGetUniformLocation(shadowShader, "shadowMap"), 1);

	testModel->Draw(shadowShader);


	


	/*
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Window::resize_callback(window, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles


	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Use the shader of programID
	//glUseProgram(shaderProgram);
	glUseProgram(testShader);
	glm::mat4 projection = Window::P;
	glm::mat4 view = Window::V;
	glUniformMatrix4fv(glGetUniformLocation(testShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(testShader, "view"), 1, GL_FALSE, glm::value_ptr(view));

	// Draw the loaded model
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// It's a bit too big for our scene, so scale it down
	glUniformMatrix4fv(glGetUniformLocation(testShader, "model"), 1, GL_FALSE, glm::value_ptr(model));

	testModel->Draw(testShader);
	*/


	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_K) {
			audioEngine->setChannelVolume(0, 2.0f);
		}
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

}