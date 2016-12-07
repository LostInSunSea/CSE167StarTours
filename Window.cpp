#include "window.h"
#include "Node.h"
#include "Group.h"
#include "Component.h"
#include "BoundingBox.h"
#include "MatrixTransformation.h"
#include "Model.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>

const char* window_title = "Galaxy Battles: Episode V - The Imperium's Counter Attack";
Cube * cube;
GLint shaderProgram;
GLint testShader;
GLint depthShader;
GLint shadowShader;
GLint skyboxShader;
GLint quadShader;


MatrixTransformation * world;
MatrixTransformation * terrainMT;
MatrixTransformation * testObj;
MatrixTransformation * testRot;
Camera * camera;

Model * terrain;
Model * testModel;
AudioEngine * audioEngine=new AudioEngine();
SkyBox * skyboxObj = new SkyBox();
BoundingBox * testBox;
BoundingBox * testBox1;

double Window::prevx;
double Window::prevy;
DirLight * sun;

//global
GLuint DepthFrameBuffer = 0;
GLuint depthTexture;

//quad to draw shadow map
GLuint quadVAO;
GLuint quadVBO;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"
#define TEST_VERTEX_SHADER "../test.vert"
#define TEST_FRAGMENT_SHADER "../test.frag"
#define DEPTH_VERTEX_SHADER "../depth.vert"
#define DEPTH_FRAGMENT_SHADER "../depth.frag"
#define SHADOW_VERTEX_SHADER "../shadow.vert"
#define SHADOW_FRAGMENT_SHADER "../shadow.frag"
#define SKYBOX_VERTEX_SHADER_PATH "../skyShader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../skyShader.frag"
#define QUAD_VERTEX_SHADER_PATH "../quad.vert"
#define QUAD_FRAGMENT_SHADER_PATH "../quad.frag"

#define SHADOW_MAP_RES 2048


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

void Window::initialize_objects()
{

	audioEngine->init(cam_pos);
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	testShader = LoadShaders(TEST_VERTEX_SHADER, TEST_FRAGMENT_SHADER);
	depthShader = LoadShaders(DEPTH_VERTEX_SHADER, DEPTH_FRAGMENT_SHADER);
	shadowShader = LoadShaders(SHADOW_VERTEX_SHADER, SHADOW_FRAGMENT_SHADER);
	skyboxShader = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
	quadShader = LoadShaders(QUAD_VERTEX_SHADER_PATH, QUAD_FRAGMENT_SHADER_PATH);

	skyboxObj->init();

	//testModel = new Model("../Assets/Models/nanosuit/nanosuit.obj");
	//testModel = new Model("../Assets/Models/snowspeeder/Star Wars Snowspeeder.obj");
	//testModel = new Model("../Assets/Models/grass/grassCube.obj");
	//testModel = new Model("../Assets/Models/scene/scene.obj");

	//testModel = new Model("../Assets/Models/snowspeeder2/snowSpeederv2.obj");
	//testModel = new Model("../Assets/Models/scene/scene.obj");
	testModel = new Model("../Assets/Models/snowspeeder2/snowSpeederv2.obj");
	terrain = new Model("../Assets/Models/terrain/terrain.obj");

	world = new MatrixTransformation();
	testObj = new MatrixTransformation();
	testRot = new MatrixTransformation();
	terrainMT = new MatrixTransformation();
	camera = new Camera();
	world->addChild(testObj);
	world->addChild(terrainMT);
	terrainMT->addChild(terrain);
	testObj->addChild(testRot);
	testRot->addChild(testModel);
	testObj->addChild(camera);

	terrainMT->M = glm::scale(glm::mat4(1.0f), glm::vec3(5, 1, 5));
	testObj->M = glm::translate(glm::mat4(1.0f), glm::vec3(0, 10, 0));




	testBox = new BoundingBox();
	testBox1 = new BoundingBox();

	sun = new DirLight(glm::vec3(-3, -9, 0), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.3f, 0.3f, 0.3f));


	

	//set up depth texture
	glGenFramebuffers(1, &DepthFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, DepthFrameBuffer);

	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_RES, SHADOW_MAP_RES, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
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

	//setup quad
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	GLfloat quadData[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadData), quadData, GL_STATIC_DRAW);
	glBindVertexArray(0);
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

	testRot->M = glm::rotate(testRot->M, 0.1f, glm::vec3(0, 1, 0));
	testObj->M = glm::translate(testObj->M, glm::vec3(0, 0, -0.1f));
	world->update(glm::mat4(1.0f));
}

void Window::display_callback(GLFWwindow* window)
{
	
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
	//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// It's a bit too big for our scene, so scale it down
	//draw the buffer
	glBindFramebuffer(GL_FRAMEBUFFER, DepthFrameBuffer);
	glViewport(0, 0, SHADOW_MAP_RES, SHADOW_MAP_RES);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Cull back-facing triangles

						 // Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw model depthshader
	world->draw(model, depthShader);
	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Window::resize_callback(window, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles
						 // Clear the color and depth buffers
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw the skybox
	skyboxObj->drawSkyBox();
	//draw model shadow shader
	//testModel->draw(model, shadowShader);
	world->draw(model, shadowShader);
	
	/*
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(model, glm::vec3(-1.0f, -1.0f, -1.0f)); // Translate it down a bit so it's at the center of the scene
	trans = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));	// It's a bit too big for our scene, so scale it down
															//Draw depth buffer
	testBox->draw(trans,shaderProgram);
	*/
	//setup box size
	glm::mat4 trans = glm::mat4(1.0f);
	glm::vec3 minVec = testModel->getMinVec();
	glm::vec3 maxVec = testModel->getMaxVec();
	glm::vec3 scaleVec = maxVec - minVec;
	trans = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
	trans = glm::scale(trans, scaleVec);	// It's a bit too big for our scene, so scale it down
	testBox->draw(trans, shaderProgram);
	//draw second box
	glm::mat4 trans1 = glm::mat4(1.0f);
	trans1 = glm::translate(model, glm::vec3(4.0f, 4.0f, 4.0f));
	trans1 = glm::scale(trans1, scaleVec);	// It's a bit too big for our scene, so scale it down
	testBox1->draw(trans1, shaderProgram);




	// display quad


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 256, 256);
	glUseProgram(quadShader);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	// Set our "renderedTexture" sampler to user Texture Unit 0
	glUniform1i(glGetUniformLocation(quadShader, "quadtexture"), 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

	glDisableVertexAttribArray(0);


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
		if (key == GLFW_KEY_T) {
			/*
			//test 1 no
			testBox->setMinExtents(glm::vec3(0.0f, 0.0f, 0.0f));
			testBox->setMaxExtents(glm::vec3(1.0f, 1.0f, 1.0f));
			testBox1->setMinExtents(glm::vec3(1.0f, 1.0f, 1.0f));
			testBox1->setMaxExtents(glm::vec3(2.0f, 2.0f, 2.0f));
			testBox->aabbTest(testBox1);
			//test 2 no
			testBox->setMinExtents(glm::vec3(0.0f, 0.0f, 0.0f));
			testBox->setMaxExtents(glm::vec3(1.0f, 1.0f, 1.0f));
			testBox1->setMinExtents(glm::vec3(1.0f, 0.0f, 0.0f));
			testBox1->setMaxExtents(glm::vec3(2.0f, 1.0f, 1.0f));
			testBox->aabbTest(testBox1);

			//test 3 yes
			testBox->setMinExtents(glm::vec3(0.0f, 0.0f, 0.0f));
			testBox->setMaxExtents(glm::vec3(1.0f, 1.0f, 1.0f));
			testBox1->setMinExtents(glm::vec3(0.0f, 0.5f, 0.0f));
			testBox1->setMaxExtents(glm::vec3(1.0f, 1.5f, 1.0f));
			testBox->aabbTest(testBox1);
			*/
			testBox->aabbTest(testBox1);

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
	glm::vec3 direction;
	float pixel_diff;
	float rot_angle, zoom_factor;
	glm::vec3 curPoint;

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		float angle;
		// Perform horizontal (y-axis) rotation
		angle = (float)(prevx - xpos) / 100.0f;
		cam_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(cam_pos, 1.0f));
		cam_up = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(cam_up, 1.0f));
		//Now rotate vertically based on current orientation
		angle = (float)(ypos - prevy) / 100.0f;
		glm::vec3 axis = glm::cross((cam_pos - cam_look_at), cam_up);
		cam_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(cam_pos, 1.0f));
		cam_up = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(cam_up, 1.0f));
		// Now update the camera
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}

	prevx = xpos;
	prevy = ypos;
}

/*

*/
