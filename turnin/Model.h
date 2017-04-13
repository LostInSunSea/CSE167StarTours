#pragma once
#include "Component.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <SOIL/SOIL.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Model:
	public Component
{
private:
	std::vector<Mesh> meshes;
	std::string directory;
	
	std::vector<Texture> textures_loaded;

	void loadModel(std::string path);

	void processNode(aiNode * node, const aiScene * scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	//max
	float maxX=-INFINITY;
	float maxY=-INFINITY;
	float maxZ=-INFINITY;
	//min
	float minX=INFINITY;
	float minY=INFINITY;
	float minZ=INFINITY;

public:
	Model();
	Model(GLchar * path);
	~Model();

	void draw(GLint shader);
	void draw(glm::mat4 trans, GLint shader);
	glm::vec3 getMinVec();
	glm::vec3 getMaxVec();
};

