#pragma once
#define GLEW_STATIC
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader{
private:
	//The Program ID
	GLuint programID;
public:
	//Constructor that reads and builds the constructor
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* gPath);
	//Use the program
	void prgUse();
	GLuint getProgramID() { return programID; }
};