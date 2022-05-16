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
		u32 shader_type;
		u32 id;
	};


	class GLProgram {
		protected:
			u32 compileShader(String& filename, i32 shaderType);
			GLuint compileShaderFromString(std::string shaderData, GLenum shaderType);
			u32 makeProgram(Array<ShaderFile> shaders);
			u32 _id;

			Array<ShaderFile> _shaders; // Need to hold onto the info on shaders to change them on the go

		public:
			u32 numOfLights = 0;

			GLProgram(Array<ShaderFile> shaders);
			void use();

			inline u32 getProgramId() { return _id; };

			void setBool(const String& name, bool value) const;
			void setInt(const String& name, i32 value) const;
			void setFloat(const String& name, f32 value) const;
			void set3Float(const String& name, f32 x, f32 y, f32 z) const;
			inline void set3Float(const String& name, vec3& v) const { set3Float(name, v.x, v.y, v.z);};
			inline void set3Float(const String& name, const vec3& v) const { set3Float(name, v.x, v.y, v.z);};
			void set4Float(const String& name, f32 x, f32 y, f32 z, f32 w) const;
			void set4Matrix(const String& name, mat4 mat) const;

			int getUniformLocation(const String& name);
			int getAtrributeLocation(const String& name);
			void printAttribInfo();

			// The change of the num of the light sources requires 
			void changeLightNum(i32 num_of_light);

	};
}
