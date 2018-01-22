#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
{
	this->meshVertices = vertices;
	this->meshIndices = indices;
	this->meshTextures = textures;
	
	this->setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, this->meshVertices.size() * sizeof(Vertex), &this->meshVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->meshIndices.size() * sizeof(GLuint), &this->meshIndices[0], GL_STATIC_DRAW);

	//Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	//Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex,vtxNormal));
	//Texture Positions
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, vtxTexCoords));

	glBindVertexArray(0);
}

void Mesh::meshDraw(Shader shader, const GLuint skybox)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint reflectNr = 1;
	for (GLuint i = 0; i < this->meshTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
										  // Retrieve texture number (the N in diffuse_textureN)
		std::stringstream ss;
		std::string number;
		std::string name = this->meshTextures[i].texType;
		if (name == "texture_diffuse")
		{
			ss << diffuseNr++; // Transfer GLuint to stream
		}
		else if (name == "texture_specular")
		{
			ss << specularNr++; // Transfer GLuint to stream
		}
		else if (name == "texture_reflect")
		{
			ss << reflectNr++; // Transfer GLuint to stream
		}
		number = ss.str();

		glUniform1i(glGetUniformLocation(shader.getProgramID(), (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, meshTextures[i].texID);
	}
	glActiveTexture(GL_TEXTURE3);
	glUniform1i(glGetUniformLocation(shader.getProgramID(), "skybox"), 3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, meshIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::meshWireDraw(Shader shader)
{
	// Draw mesh
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->meshIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}