/*
* A class encapsulating shaders.
* Stores, compiles and links vertex and fragment shaders given the paths to the shader files
*/

#pragma once

#include <cstdio>

#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

#include <string>




class Shader
{
private:

	/*void checkCompileErrors(unsigned int shader, std::string type);*/

public:

	// ID of the program 
	unsigned int ID;

	// Constructor reads and builds the shader
	Shader(const char* vertex_file_path, const char* fragment_file_path);

	// Use/activate the shader
	void use();

	// Setters
	void setBool(const std::string& name, bool value) const;

	void setInt(const std::string& name, int value) const;

	void setFloat(const std::string& name, float value) const;

	void set4Float(const std::string& name, float x, float y, float z, float w) const;

	// Getters
	int getUniformLocation(const std::string& name);
	int getAtrributeLocation(const std::string& name);

};
