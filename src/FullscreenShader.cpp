#include <FullscreenShader.h>
#include <Utils.h>

using namespace NoxEngine;

FullscreenShader::FullscreenShader(const String shader_src, Array<TextureInput> texture_inputs, u32 frame_width, u32 frame_height)
	:GLProgram(Array<ShaderFile>{
		{"assets/shaders/fullScreenShader.vert", GL_VERTEX_SHADER, 0},
		{shader_src, GL_FRAGMENT_SHADER, 0}
	}),
	fragment_shader(shader_src),
	texture_inputs(texture_inputs),
	frame_width(frame_width),
	frame_height(frame_height)
{

	saveState();

	glGenFramebuffers(1, &framebuffer_id);
	glGenTextures(1, &texture_id);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame_width, frame_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		LOG_DEBUG("Fullscreen post-processor successfully built : %s", shader_src.c_str());
	} else {
		LOG_DEBUG("Failed to build fullscreen post-processor : %s", shader_src.c_str());
	}

	restoreState();
}

void FullscreenShader::saveState() {
	glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &current_bound_texture);
	glGetIntegerv(GL_DRAW_FRAMEBUFFER, &current_bound_framebuffer);
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_bound_program);
}

void FullscreenShader::restoreState() {
	glBindTexture(GL_TEXTURE_2D, current_bound_texture);
	glBindFramebuffer(GL_FRAMEBUFFER, current_bound_framebuffer);
	glUseProgram(current_bound_program);

}

void FullscreenShader::draw() {
	saveState();
	use();

	for(u32 i = 0; i < texture_inputs.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + texture_inputs[i].texture_location);
		glBindTexture(GL_TEXTURE_2D, texture_inputs[i].texture_id);
	}

	// the vertex shader is common across all fullscreen shaders
	// it just contains the coords for a quad and here it renders that quad
	glDrawElements(0, 6, GL_UNSIGNED_INT, 0);

	restoreState();
}

