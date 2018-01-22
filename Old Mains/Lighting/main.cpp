#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <cmath>
#include <SOIL\SOIL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();
void texChange();
void centerWindow (GLFWwindow* window,  int width, int height);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

GLfloat texValue = 0.5f, lastX = WIDTH/2.0f, lastY = HEIGHT/2.0f, fov = 45.0f;

//Key states
bool keys[1024], firstMouse = true;

//Time trackers
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// The MAIN function, from here we start the application and run the game loop
int main()
{

	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	centerWindow(window, width, height);
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);

	Shader lightingShader("vertex.glsl", "fragment.glsl");
	Shader lampShader("lampVertex.glsl", "lampFragment.glsl");

	GLint vertexHztlOffsetLocation;

	// Set up vertex data (and buffer(s)) and attribute pointers
	//GLfloat vertices[] = {
	//  // First triangle
	//   0.5f,  0.5f,  // Top Right
	//   0.5f, -0.5f,  // Bottom Right
	//  -0.5f,  0.5f,  // Top Left 
	//  // Second triangle
	//   0.5f, -0.5f,  // Bottom Right
	//  -0.5f, -0.5f,  // Bottom Left
	//  -0.5f,  0.5f   // Top Left
	//}; 
	GLfloat vertices[] = {
		// Positions           // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO


	//Light VAO
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Change texture wrapping method: default is repeat
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//for GL_CLAMP_TO_BORDER
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//color of border must be specified
	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//Setting texture filtering for magnifying and minifying operations
	int wTex, hTex;

/*	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* image1 = SOIL_load_image("Assets/Textures/container.jpg", &wTex, &hTex, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wTex, hTex, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);

	//free image data and unbind texture object
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* image2 = SOIL_load_image("Assets/Textures/awesomeface.png", &wTex, &hTex, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wTex, hTex, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);

	//free image data and unbind texture object
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);
*/
	GLuint diffuseMap;
	glGenTextures(1, &diffuseMap);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);

	unsigned char* image3 = SOIL_load_image("Assets/Textures/container2.png", &wTex, &hTex, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wTex, hTex, 0, GL_RGB, GL_UNSIGNED_BYTE, image3);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image3);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint specularMap;
	glGenTextures(1, &specularMap);
	glBindTexture(GL_TEXTURE_2D, specularMap);

	unsigned char* image4 = SOIL_load_image("Assets/Textures/container2_specular.png", &wTex, &hTex, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wTex, hTex, 0, GL_RGB, GL_UNSIGNED_BYTE, image4);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image4);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint emissionMap;
	glGenTextures(1, &emissionMap);
	glBindTexture(GL_TEXTURE_2D, emissionMap);

	unsigned char* image5 = SOIL_load_image("Assets/Textures/matrix.jpg", &wTex, &hTex, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wTex, hTex, 0, GL_RGB, GL_UNSIGNED_BYTE, image5);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image5);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	lightingShader.prgUse();
	glUniform1i(glGetUniformLocation(lightingShader.getProgramID(), "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.getProgramID(), "material.specular"), 1);
	glUniform1i(glGetUniformLocation(lightingShader.getProgramID(), "material.emission"), 2);



	// Uncommenting this call will result in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		do_movement();
		//texChange();

		// Render
		// Clear the colorbuffer
		glClearColor(0.05f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*
		TEXTURE STUFF

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(lightingShader.getProgramID(), "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(lightingShader.getProgramID(), "ourTexture2"), 1);

		glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "value"), texValue);
		*/

		lightingShader.prgUse();
		//Light Color Change

		//glm::vec3 lightColor;
		//lightColor.x = glm::sin(glfwGetTime() * 2.0f);
		//lightColor.y = glm::sin(glfwGetTime() * 0.7f);
		//lightColor.z = glm::sin(glfwGetTime() * 1.3f);

		//glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		//glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		glm::vec3 lightColor(1.0f);
		glm::vec3 diffuseColor(0.8f, 0.8f, 0.8f);
		glm::vec3 ambientColor(0.2f, 0.2f, 0.2f);

		//Directional Light
		GLint lightDirPos = glGetUniformLocation(lightingShader.getProgramID(), "dirLight.direction");
		GLint lightAmbientLoc = glGetUniformLocation(lightingShader.getProgramID(), "dirLight.ambient");
		GLint lightDiffuseLoc = glGetUniformLocation(lightingShader.getProgramID(), "dirLight.diffuse");
		GLint lightSpecularLoc = glGetUniformLocation(lightingShader.getProgramID(), "dirLight.specular");

		glUniform3f(lightDirPos, -0.2f, -1.0f, -0.3f);
		glUniform3f(lightAmbientLoc, 0.02f, 0.02f, 0.02f);
		glUniform3f(lightDiffuseLoc, 0.3f, 0.3f, 0.3f);
		glUniform3f(lightSpecularLoc, 0.4f, 0.4f, 0.4f);

		GLint lightPosLoc;
		//Point Lights
		{
			lightPosLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[0].position");
			lightAmbientLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[0].ambient");
			lightDiffuseLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[0].diffuse");
			lightSpecularLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[0].specular");

			glUniform3f(lightPosLoc, pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);

			glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "pointLights[0].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "pointLights[0].linear"), 0.09);
			glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "pointLights[0].quadratic"), 0.032);

			glUniform3f(lightAmbientLoc, 0.05f, 0.03f, 0.0f);
			glUniform3f(lightDiffuseLoc, 0.78f, 0.05f, 0.08f);
			glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);


			lightPosLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[1].position");
			lightAmbientLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[1].ambient");
			lightDiffuseLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[1].diffuse");
			lightSpecularLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[1].specular");

			glUniform3f(lightPosLoc, pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);

			glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "pointLights[1].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "pointLights[1].linear"), 0.09);
			glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "pointLights[1].quadratic"), 0.032);

			glUniform3f(lightAmbientLoc, 0.025f, 0.02f, 0.09f);
			glUniform3f(lightDiffuseLoc, 0.98f, 0.08f, 0.05f);
			glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

			lightPosLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[2].position");
			lightAmbientLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[2].ambient");
			lightDiffuseLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[2].diffuse");
			lightSpecularLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[2].specular");

			glUniform3f(lightPosLoc, pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);

			glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "pointLights[2].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "pointLights[2].linear"), 0.09);
			glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "pointLights[2].quadratic"), 0.032);

			glUniform3f(lightAmbientLoc, 0.05f, 0.082f, 0.09f);
			glUniform3f(lightDiffuseLoc, 0.8f, 0.4f, 0.0f);
			glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

			lightPosLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[3].position");
			lightAmbientLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[3].ambient");
			lightDiffuseLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[3].diffuse");
			lightSpecularLoc = glGetUniformLocation(lightingShader.getProgramID(), "pointLights[3].specular");

			glUniform3f(lightPosLoc, pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);

			glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "pointLights[3].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "pointLights[3].linear"), 0.09);
			glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "pointLights[3].quadratic"), 0.032);

			glUniform3f(lightAmbientLoc, 0.0f, 0.09f, 0.1f);
			glUniform3f(lightDiffuseLoc, 0.3f, 0.5f, 0.9f);
			glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
		}

		//Spotlight

		lightPosLoc = glGetUniformLocation(lightingShader.getProgramID(), "spotLight.position");
		lightDirPos = glGetUniformLocation(lightingShader.getProgramID(), "spotLight.direction");
		lightAmbientLoc = glGetUniformLocation(lightingShader.getProgramID(), "spotLight.ambient");
		lightDiffuseLoc = glGetUniformLocation(lightingShader.getProgramID(), "spotLight.diffuse");
		lightSpecularLoc = glGetUniformLocation(lightingShader.getProgramID(), "spotLight.specular");

		glUniform3f(lightPosLoc, camera.position.x, camera.position.y, camera.position.z);
		glUniform3f(lightDirPos, camera.front.x, camera.front.y, camera.front.z);
		glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "spotLight.outerCutOff"), glm::cos(glm::radians(17.5f)));

		glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "spotLight.linear"), 0.18);
		glUniform1f(glGetUniformLocation(lightingShader.getProgramID(), "spotLight.quadratic"), 0.008);

		glUniform3f(lightAmbientLoc, 0.01f, 0.01f, 0.01f);
		glUniform3f(lightDiffuseLoc, 0.7f, 0.7f, 0.7f);
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

		//Materials
		GLint matSpecularLoc = glGetUniformLocation(lightingShader.getProgramID(), "material.specular");
		GLint matShineLoc = glGetUniformLocation(lightingShader.getProgramID(), "material.shininess");

		glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
		glUniform1f(matShineLoc, 64.0f);

		
		GLint viewPosLoc = glGetUniformLocation(lightingShader.getProgramID(), "viewPos");
		glUniform3f(viewPosLoc, camera.position.x, camera.position.y, camera.position.z);

		//lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		//lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

		
		
		//MVP Mat4s
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		view = camera.getViewMatrix();

		projection = glm::perspective(camera.zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f);

		GLint modelLoc = glGetUniformLocation(lightingShader.getProgramID(), "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.getProgramID(), "view");
		GLint projectionLoc = glGetUniformLocation(lightingShader.getProgramID(), "projection");
		
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emissionMap);

		glBindVertexArray(VAO);
		for (GLuint i = 0; i < 10; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 0.0f * (i+1);
			model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);

		//LampVAO
		lampShader.prgUse();
		modelLoc = glGetUniformLocation(lampShader.getProgramID(), "model");
		viewLoc = glGetUniformLocation(lampShader.getProgramID(), "view");
		projectionLoc = glGetUniformLocation(lampShader.getProgramID(), "projection");
		//Set Matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw the light object (using light's vertex attributes)
		for (int i = 0; i < 4; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if(action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;
	
	camera.processMouseMovement(xoffset,yoffset);
} 

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseMovement(xoffset,yoffset);
}

void do_movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.processKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.processKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.processKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.processKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_Q])
		camera.processKeyboard(MUP, deltaTime);
	if (keys[GLFW_KEY_E])
		camera.processKeyboard(MDOWN, deltaTime);
}

void texChange()
{
	if (keys[GLFW_KEY_UP] && texValue < 1.0f)
		texValue += 0.01f;
	if (keys[GLFW_KEY_DOWN] && texValue > 0.01f)
		texValue -= 0.05f;
}

void centerWindow(GLFWwindow* window, int width, int height)
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowPos(window, (mode->width - width) / 2, (mode->height - height) / 2);
}