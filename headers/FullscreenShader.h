#pragma once

#include <Types.h>
#include <GLProgram.h>

namespace NoxEngine {

	struct TextureInput {
		u32 texture_id;
		u32 texture_location;
	};


	class FullscreenShader : public GLProgram {
		public:
			FullscreenShader(const String shader_src, Array<TextureInput> texture_inputs = Array<TextureInput>(), u32 frame_width = 0, u32 frame_height = 0);
			void saveState();
			void restoreState();
			void draw();

			inline u32 GetTexture() { return texture_id; }

		private:
			const String fragment_shader; 
			u32 frame_width;
			u32 frame_height;
			u32 texture_id;
			u32 framebuffer_id;
			Array<TextureInput> texture_inputs;


			i32 current_bound_texture;
			i32 current_bound_framebuffer;
			i32 current_bound_program;
	};

}
