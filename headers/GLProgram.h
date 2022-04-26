/*
* A class encapsulating shaders.
* Stores, compiles and links vertex and fragment shaders given the paths to the shader files
*/

#pragma once
#define GLEW_STATIC

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <Types.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace NoxEngine {

	struct ShaderFile {
		std::string filename;
		GLenum shader_type;
		GLuint id;
	};


	class GLProgram {
		private:
			GLuint compileShader(std::string& filename, GLenum shaderType);
			GLuint makeProgram(std::vector<ShaderFile> shaders);

		public:
			// ID of the program 
			u32 _id;

			// Constructor reads and builds the shader
			GLProgram(std::vector<ShaderFile> shaders);

			// Use/activate the shader
			void use();

			// Setters
			void setBool(const std::string& name, bool value) const;
			void setInt(const std::string& name, int value) const;
			void setFloat(const std::string& name, float value) const;
			void set3Float(const std::string& name, float x, float y, float z) const;
			void set4Float(const std::string& name, float x, float y, float z, float w) const;
			void set4Matrix(const std::string& name, glm::mat4 mat) const;

			// Getters
			int getUniformLocation(const std::string& name);
			int getAtrributeLocation(const std::string& name);

	};
}
