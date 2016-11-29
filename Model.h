#pragma once
#include <iostream>
#include <string>
#include <vector>
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

class Model
{
private:
	std::vector<Mesh> meshes;
	std::string directory;
	
	std::vector<Texture> textures_loaded;

	void loadModel(std::string path);

	void processNode(aiNode * node, const aiScene * scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);


public:
	Model();
	Model(GLchar * path);
	~Model();

	void Draw(GLint shader);

};

