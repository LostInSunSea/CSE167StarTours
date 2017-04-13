#include "window.h"
#include "Node.h"
#include "Group.h"
#include "Component.h"
#include "BoundingBox.h"
#include "MatrixTransformation.h"
#include "Model.h"
#include "Camera.h"
#include "Spawner.h"
#include "IntermediateBox.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <time.h>

const char* window_title = "Galaxy Battles: Episode V - The Imperium's Counter Attack";
GLint shaderProgram;
GLint depthShader;
GLint shadowShader;
GLint skyboxShader;
GLint quadShader;
GLint laserShader;
vector<BoundingBox *> listOfBoundingBoxes;



MatrixTransformation * world;
MatrixTransformation * terrainMT;
MatrixTransformation * speederMT;
MatrixTransformation * speederRot;
MatrixTransformation * laserMT;
MatrixTransformation * laserWorld;
MatrixTransformation * gunMTL;
MatrixTransformation * gunMTR;
Spawner * gun;
Camera * camera;
MatrixTransformation * terrainBoxMT;
MatrixTransformation * speederBoxMT;
MatrixTransformation * atatMT;
MatrixTransformation * turretMT;
MatrixTransformation * turretHeadMT;
MatrixTransformation * turretBaseMT;


Model * atat;
Model * laser;
Model * terrain;
Model * turretHead;
Model * turretBase;
BoundingBox * terrainBox;
Model * speederModel;
BoundingBox * speederBox;
IntermediateBox * speederBoxInt;
BoundingBox * testBox;
BoundingBox * atatBox;
MatrixTransformation * atatBoxMT;
glm::mat4 testmat = glm::mat4(1);
glm::vec3 testVec = glm::vec3(0);
bool drawBoundingBoxes=true;
AudioEngine * audioEngine=new AudioEngine();
SkyBox * skyboxObj = new SkyBox();

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
#define LASER_VERTEX_SHADER_PATH "../laser.vert"
#define LASER_FRAGMENT_SHADER_PATH "../laser.frag"

#define SHADOW_MAP_RES 8192


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

float speed = 0.1f;
float laserSpeed = 3.0f;


bool displayShadowMap = false;


int Window::manip = 0;
int Window::light = 1;
bool Window::normal = false;

int shadowStyle = 1;

void Window::initialize_objects()
{
	srand(time(NULL));
	audioEngine->init(cam_pos);
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	depthShader = LoadShaders(DEPTH_VERTEX_SHADER, DEPTH_FRAGMENT_SHADER);
	shadowShader = LoadShaders(SHADOW_VERTEX_SHADER, SHADOW_FRAGMENT_SHADER);
	skyboxShader = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
	quadShader = LoadShaders(QUAD_VERTEX_SHADER_PATH, QUAD_FRAGMENT_SHADER_PATH);
	laserShader = LoadShaders(LASER_VERTEX_SHADER_PATH, LASER_FRAGMENT_SHADER_PATH);

	cout << "shader program: " << shaderProgram << endl;
	cout << "shader depth: " << depthShader << endl;
	cout << "shader shadow: " << shadowShader << endl;
	cout << "shader skybox: " << skyboxShader << endl;
	cout << "shader quad: " << quadShader << endl;


	skyboxObj->init();

	speederModel = new Model("../Assets/Models/snowspeeder2/snowSpeederv2.obj");
	terrain = new Model("../Assets/Models/terrain/terrain3.obj");
	laser = new Model("../Assets/Models/laser/laser.obj");
	atat = new Model("../Assets/Models/atat/atat.obj");
	turretBase = new Model("../Assets/Models/turret/turretBase.obj");
	turretHead = new Model("../Assets/Models/turret/turretHead.obj");
	audioEngine->streamSound("../Assets/Sound/Ambient/Hoth.mp3",false,true);
	audioEngine->loadSound("../Assets/Sound/Ambient/Snowspeeder/Engine1.wav", false, true);
	audioEngine->loadSound("../Assets/Sound/Ambient/laser.mp3", false, false);
	audioEngine->loadSound("../Assets/Sound/Ambient/R2D2/Death.mp3", false, false);
	audioEngine->loadSound("../Assets/Sound/Music/neeley.mp3", false, false);
	audioEngine->loadSound("../Assets/Sound/Music/cantina.mp3", false, false);
	audioEngine->loadSound("../Assets/Sound/Music/James_Bond.mp3", false, false);
	audioEngine->loadSound("../Assets/Sound/Music/nyancat.mp3", false, false);



	terrainBox = new BoundingBox();

	world = new MatrixTransformation();
	terrainMT = new MatrixTransformation();
	speederMT = new MatrixTransformation();
	speederRot = new MatrixTransformation();
	laserMT = new MatrixTransformation();
	laserWorld = new MatrixTransformation();
	gunMTL = new MatrixTransformation();
	gunMTR = new MatrixTransformation();
	gun = new Spawner(laserMT);
	terrainBoxMT = new MatrixTransformation();
	speederBoxMT = new MatrixTransformation();
	speederBoxInt = new IntermediateBox();
	atatMT = new MatrixTransformation();
	turretMT = new MatrixTransformation();
	turretHeadMT = new MatrixTransformation();
	turretBaseMT = new MatrixTransformation();
	camera = new Camera();
	atatBox = new BoundingBox();
	atatBoxMT = new MatrixTransformation();


	testBox = new BoundingBox();
	testBox->setNonStatic(false);

	
	world->addChild(terrainMT);
	terrainMT->addChild(terrain);
	terrainMT->addChild(terrainBoxMT);
	terrainBoxMT->addChild(terrainBox);

	speederRot->M = glm::rotate(speederRot->M, 90.0f / 180.0f * glm::pi<float>(), glm::vec3(0, 1, 0));
	speederRot->addChild(speederModel);
	speederMT->addChild(speederRot);
	speederMT->addChild(camera);
	speederMT->addChild(speederBoxMT);
	speederMT->addChild(testBox);
	speederBoxMT->addChild(speederBoxInt);
	world->addChild(speederMT);

	//properly make transformation for speeder
	glm::mat4 trans2 = glm::mat4(1.0f);
	glm::vec3 minVec2 = speederModel->getMinVec();
	glm::vec3 maxVec2 = speederModel->getMaxVec();
	glm::vec3 scaleVec2 = maxVec2 - minVec2;
	speederBoxMT->M = glm::scale(trans2, scaleVec2);

	//properly create the transformation matrix for the box
	glm::mat4 trans = glm::mat4(1.0f);
	glm::vec3 minVec = terrain->getMinVec();
	glm::vec3 maxVec = terrain->getMaxVec();
	glm::vec3 scaleVec = maxVec - minVec;
	terrainBoxMT->M = glm::scale(trans, scaleVec);
	terrainMT->M = glm::scale(glm::mat4(1.0f), glm::vec3(20, 1, 20));
	speederMT->M = glm::translate(glm::mat4(1.0f), glm::vec3(50, 50, 50));

	//laserWorld->addChild(laserMT);
	laserMT->addChild(laser);
	//laserMT->M = glm::translate(glm::mat4(1.0f), glm::vec3(0, 50, 0));
	gunMTL->M = glm::translate(glm::mat4(1.0f), glm::vec3(1.7f, -0.4f, -2.0f));
	gunMTR->M = glm::translate(glm::mat4(1.0f), glm::vec3(-1.7f, -0.4f, -2.0f));
	gunMTL->addChild(gun);
	gunMTR->addChild(gun);
	speederMT->addChild(gunMTL);
	speederMT->addChild(gunMTR);

	atatMT->M = glm::scale(atatMT->M, glm::vec3(10, 10, 10));
	atatMT->M = glm::translate(atatMT->M, glm::vec3(0, -0.1, 0));
	atatMT->addChild(atat);
	atatMT->addChild(atatBoxMT);
	atatBoxMT->addChild(atatBox);
	world->addChild(atatMT);

	//properly make transformation for speeder
	glm::mat4 trans3 = glm::mat4(1.0f);
	glm::vec3 minVec3 = atat->getMinVec();
	glm::vec3 maxVec3 = atat->getMaxVec();
	glm::vec3 scaleVec3 = maxVec3 - minVec3;
	trans3 = glm::translate(trans3, glm::vec3(0.0f,4.0f,0.0f));
	atatBoxMT->M = glm::scale(trans3, scaleVec3);

	turretBaseMT->addChild(turretBase);
	turretHeadMT->addChild(turretHead);
	turretMT->addChild(turretBaseMT);
	turretMT->addChild(turretHeadMT);

	turretMT->M = glm::scale(turretMT->M, glm::vec3(2, 2, 2));
	turretMT->M = glm::translate(turretMT->M, glm::vec3(0, 4, 0));


	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			int degree = rand() % 360;
			MatrixTransformation * turretTemp = new MatrixTransformation();
			int randomx = rand() % 50;
			int randomy = rand() % 50;
			turretTemp->M = glm::rotate(glm::mat4(1), (float)(degree) / 180.0f * glm::pi<float>(), glm::vec3(0, 1, 0));
			turretTemp->M = glm::translate(glm::mat4(1), glm::vec3(-300 + (100 * i) + randomx, 0, 300 - (100 * j) + randomy)) * turretTemp->M;
			turretTemp->addChild(turretMT);
			world->addChild(turretTemp);
		}
		
	}
	
	
	
	//world->addChild(turretMT);

	listOfBoundingBoxes.push_back(terrainBox);
	listOfBoundingBoxes.push_back(atatBox);
	sun = new DirLight(glm::vec3(-3, -3, -3), glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.3f, 0.3f, 0.3f));


	

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

	audioEngine->playSound("../Assets/Sound/Ambient/Hoth.mp3", glm::vec3(1), glm::vec3(1), 1.0f);
	audioEngine->playSound("../Assets/Sound/Ambient/Snowspeeder/Engine1.wav", glm::vec3(1), glm::vec3(1), 2.0f);
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
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 10000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	//update
	speederMT->M = glm::translate(speederMT->M, glm::vec3(0, 0, -speed));

	for (Node * n : laserWorld->allNodes)
	{
		if (MatrixTransformation * mt = dynamic_cast<MatrixTransformation *>(n))
		{
			mt->M = glm::translate(mt->M, glm::vec3(0, 0, -(laserSpeed + speed)));
		}
	}
	
	//testRot->M = glm::rotate(testRot->M, 0.1f, glm::vec3(0, 1, 0));
	//testObj->M = glm::translate(testObj->M, glm::vec3(0, 0, -0.1f));
	testBox->setHeight(speederBoxInt->getHeight());
	testBox->setWidth(speederBoxInt->getWidth());
	testBox->setLength(speederBoxInt->getLength());
	world->update(glm::mat4(1.0f));
	laserWorld->update(glm::mat4(1.0f));
	checkAllBoundingBoxes();
	gun->spawn = false;
}

void Window::display_callback(GLFWwindow* window)
{
	
	glm::mat4 model = glm::mat4(1.0f);
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
	world->draw(model, shadowShader);

	laserWorld->draw(model, laserShader);

	// display quad

	if (displayShadowMap)
	{
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
	}
	


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
		if (key == GLFW_KEY_M) {
			displayShadowMap = !displayShadowMap;
		}
		if (key == GLFW_KEY_W) {
			speed += 0.1f;
		}
		if (key == GLFW_KEY_S) {
			speed -= 0.1f;
		}
		if (key == GLFW_KEY_SPACE) {
			speed = 0;
		}
		if (key == GLFW_KEY_A) {
			speederMT->M = glm::rotate(speederMT->M, (float)(1) / 180.0f * glm::pi<float>(), glm::vec3(0, 1, 0));
		}
		if (key == GLFW_KEY_D) {
			speederMT->M = glm::rotate(speederMT->M, (float)(-1) / 180.0f * glm::pi<float>(), glm::vec3(0, 1, 0));
		}
		if (key == GLFW_KEY_C) {
			drawBoundingBoxes = !drawBoundingBoxes;
			testBox->setDraw(drawBoundingBoxes);
			speederBoxInt->setDraws();
			for (int i = 0; i < listOfBoundingBoxes.size(); i++) {
				listOfBoundingBoxes[i]->setDraw(drawBoundingBoxes);
			}

		}
		if (key == GLFW_KEY_1) {
			shadowStyle = 1;
		}
		if (key == GLFW_KEY_2) {
			shadowStyle = 2;
		}
		if (key == GLFW_KEY_6) {
			audioEngine->playSound("../Assets/Sound/Music/cantina.mp3",glm::vec3(1),glm::vec3(1),4.0);
		}
		if (key == GLFW_KEY_7) {
			audioEngine->playSound("../Assets/Sound/Music/neeley.mp3", glm::vec3(1), glm::vec3(1), 4.0);
		}
		if (key == GLFW_KEY_8) {
			audioEngine->playSound("../Assets/Sound/Music/nyancat.mp3", glm::vec3(1), glm::vec3(1), 4.0);
		}
		if (key == GLFW_KEY_5) {
			audioEngine->playSound("../Assets/Sound/Music/James_Bond.mp3", glm::vec3(1), glm::vec3(1), 4.0);
		}
	}
	if (action == GLFW_REPEAT)
	{
		if (key == GLFW_KEY_W) {
			speed += 0.1f;
		}
		if (key == GLFW_KEY_S) {
			speed -= 0.1f;
		}
		if (key == GLFW_KEY_A) {
			speederMT->M = glm::rotate(speederMT->M, (float)(1) / 180.0f * glm::pi<float>(), glm::vec3(0, 1, 0));
		}
		if (key == GLFW_KEY_D) {
			speederMT->M = glm::rotate(speederMT->M, (float)(-1) / 180.0f * glm::pi<float>(), glm::vec3(0, 1, 0));
		}
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		audioEngine->playSound("../Assets/Sound/Ambient/laser.mp3", glm::vec3(1), glm::vec3(1), 5.0f);
		gun->spawn = true;
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	glm::vec3 direction;
	float pixel_diff;
	float rot_angle, zoom_factor;
	glm::vec3 curPoint;

	speederMT->M = glm::rotate(speederMT->M, ((float)(ypos - prevy) / 5) / 180.0f * glm::pi<float>(), glm::vec3(1, 0, 0));
	speederMT->M = glm::rotate(speederMT->M, ((float)(prevx - xpos) / 5) / 180.0f * glm::pi<float>(), glm::vec3(0, 0, 1));

	/*
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		
	}
	*/
	

	prevx = xpos;
	prevy = ypos;
}

void Window::checkAllBoundingBoxes()
{
	bool noCol = false;
	for (int i = 0; i < listOfBoundingBoxes.size(); i++) {
		if (testBox->aabbTest(listOfBoundingBoxes[i])) {
			noCol = true;
			testBox->setColor(3);
			listOfBoundingBoxes[i]->setColor(3);
			audioEngine->playSound("../Assets/Sound/Ambient/R2D2/Death.mp3", glm::vec3(1), glm::vec3(1),3.0f);
		}
		else {
			listOfBoundingBoxes[i]->setColor(2);
		}
	}
	if (!noCol) {
		testBox->setColor(2);
	}

}

/*

*/
