#pragma once

#include <Core/Types.h>
#include <Core/GLProgram.h>
#include <Managers/IReloadableFile.h>

namespace NoxEngine {

	struct TextureInput {
		u32 texture_id;
		u32 texture_location;
	};

	class FullscreenShader : public GLProgram, public IReloadableFile {
		public:

			FullscreenShader(
				u32 frame_width = 0,
				u32 frame_height = 0,
				const char *name = "PostProcessor"
			);

			FullscreenShader(const FullscreenShader&);
			~FullscreenShader();
			FullscreenShader(
					const String shader_src,
					Array<TextureInput> texture_inputs = Array<TextureInput>(),
					u32 frame_width = 0,
					u32 frame_height = 0,
					const char *name = "PostProcessor"
			);
			void saveState();
			void restoreState();
			void draw(time_type deltaTime);
			bool IsInit() { return inited;};

			void AddInput(u32 texture_id, u32 texture_unit);
			void RemoveInput(i32 input_index);

			bool ChangeShader(String& shader_path);
			void changeTextureSize(i32 width, i32 height);

			void liveReloadFile(const char *file, LiveReloadEntry *entry) override;

			inline u32 GetTexture() { return texture_id; }
			inline const char* GetShader() const { return fragment_shader.c_str(); };
			inline const char* GetName() const { return name.c_str(); };
			inline const Array<TextureInput>& GetTextureInputs() const { return texture_inputs; }

		private:
			String fragment_shader; 
			String name;
			u32 frame_width;
			u32 frame_height;
			u32 texture_id;
			u32 framebuffer_id;
			Array<TextureInput> texture_inputs;
			bool inited;

			i32 current_bound_texture;
			i32 current_bound_framebuffer;
			i32 current_bound_program;
	};
}
