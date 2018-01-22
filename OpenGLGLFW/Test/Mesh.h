#pragma once
#define GLEW_STATIC
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "Shader.h"
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

struct Vertex {
	glm::vec3 vtxPosition;
	glm::vec3 vtxNormal;
	glm::vec2 vtxTexCoords;
};

struct Texture {
	GLuint texID;
	std::string texType;
	aiString texPath;
};

class Mesh {
public:
	std::vector<Vertex>	meshVertices;
	std::vector<GLuint> meshIndices;
	std::vector<Texture> meshTextures;

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	void meshDraw(Shader shader, GLuint skybox);
	void meshWireDraw(Shader shader);

private:
	GLuint VAO, VBO, EBO;
	void setupMesh();

};