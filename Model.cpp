#include "Model.h"
#include "Window.h"
extern DirLight* sun;
extern glm::vec3 cam_pos;
extern GLuint depthTexture;
extern GLuint DepthFrameBuffer;
extern GLint depthShader;
extern GLint shadowShader;

extern GLint depthShader;
extern GLint shadowShader;

GLint TextureFromFile(const char* path, std::string directory)
{
	//Generate texture ID and load texture data 
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}

Model::Model()
{
}

Model::Model(GLchar * path)
{
	loadModel(path);
}


Model::~Model()
{
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(processMesh(mesh, scene));
	}
	// Then do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}


Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		//---------------------------------------
		//x
		if (vertex.Position.x>maxX) {
			maxX = vertex.Position.x;
		}
		else if (vertex.Position.x<minX) {
			minX = vertex.Position.x;
		}
		//y
		if (vertex.Position.y>maxY) {
			maxY = vertex.Position.y;
		}
		else if (vertex.Position.y<minY) {
			minY = vertex.Position.y;
		}
		//y
		if (vertex.Position.z>maxZ) {
			maxZ = vertex.Position.z;
		}
		else if (vertex.Position.z<minZ) {
			minZ = vertex.Position.z;
		}
		//---------------------------------------

		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}
	// Process indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// Process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = this->loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		GLboolean skip = false;
		for (GLuint j = 0; j < textures_loaded.size(); j++)
		{
			if (textures_loaded[j].path == str)
			{
				textures.push_back(textures_loaded[j]);	
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->textures_loaded.push_back(texture);  // Add to loaded textures
		}
	}
	return textures;
}

void Model::Draw(GLint shader)
{
	for (GLuint i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::Draw(glm::mat4 model, GLint shader)
{
	glUseProgram(shader);
	glm::vec3 lightInvDir = -(sun->dir);
	//need to fine tune/gen automatically
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	//BASED OFF EACH OBJECT
	glm::mat4 depthModelMatrix = model;
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	if (shader == depthShader)
	{

		glUniformMatrix4fv(glGetUniformLocation(shader, "depthMVP"), 1, GL_FALSE, &depthMVP[0][0]);
		Draw(shader);
		return;
	}

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	glm::mat4 depthBiasMVP = biasMatrix*depthMVP;
	glm::mat4 projection = Window::P;
	glm::mat4 view = Window::V;

	//just throw it at the end
	if (shader == shadowShader) {
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shader, "DepthBiasMVP"), 1, GL_FALSE, &depthBiasMVP[0][0]);

		glUniform3f(glGetUniformLocation(shader, "viewPos"), cam_pos.x, cam_pos.y, cam_pos.z);
		glUniform3f(glGetUniformLocation(shader, "dirLight.direction"), sun->dir.x, sun->dir.y, sun->dir.z);
		glUniform3f(glGetUniformLocation(shader, "dirLight.ambient"), sun->ambient.x, sun->ambient.y, sun->ambient.z);
		glUniform3f(glGetUniformLocation(shader, "dirLight.diffuse"), sun->diffuse.x, sun->diffuse.y, sun->diffuse.z);
		glUniform3f(glGetUniformLocation(shader, "dirLight.specular"), sun->specular.x, sun->specular.y, sun->specular.z);
		glActiveTexture(GL_TEXTURE15);
		glUniform1i(glGetUniformLocation(shader, "shadowMap"), 15);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		Draw(shader);
	}
}

glm::vec3 Model::getMinVec()
{
	glm::vec3 minVec = glm::vec3(minX, minY, minZ);
	return minVec;
}

glm::vec3 Model::getMaxVec()
{
	glm::vec3 maxVec = glm::vec3(maxX, maxY, maxZ);
	return maxVec;
}


