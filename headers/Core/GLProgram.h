/*
* A class encapsulating shaders.
* Stores, compiles and links vertex and fragment shaders given the paths to the shader files
*/

#pragma once

#include <glad/glad.h>
#include <string>
#include <Core/Types.h>

namespace NoxEngine {

	struct ShaderFile {
		String filename;
		GLenum shader_type;
		GLuint id;
	};


	class GLProgram {
		private:
			GLuint compileShader(String& filename, GLenum shaderType);
			GLuint makeProgram(Array<ShaderFile> shaders);
			u32 _id;

		public:
			GLProgram(Array<ShaderFile> shaders);
			void use();

			void setBool(const String& name, bool value) const;
			void setInt(const String& name, i32 value) const;
			void setFloat(const String& name, f32 value) const;
			void set3Float(const String& name, f32 x, f32 y, f32 z) const;
			inline void set3Float(const String& name, vec3& v) const { set3Float(name, v.x, v.y, v.z);};
			void set4Float(const String& name, f32 x, f32 y, f32 z, f32 w) const;
			void set4Matrix(const String& name, mat4 mat) const;

			int getUniformLocation(const String& name);
			int getAtrributeLocation(const String& name);
			void printAttribInfo();

	};
}
