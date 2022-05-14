#include <FullscreenShader.h>
#include <Utils/Utils.h>
#include <Utils/MemAllocator.h>
#include <Managers/LiveReloadManager.h>

using namespace NoxEngine;



FullscreenShader::FullscreenShader(u32 frame_width, u32 frame_height, const char *name)
	:GLProgram(Array<ShaderFile>{
		{"assets/shaders/fullScreenShader.vert", GL_VERTEX_SHADER, 0},
	}),
	texture_inputs(),
	frame_width(frame_width),
	frame_height(frame_height),
	current_bound_program(0),
	current_bound_texture(0),
	current_bound_framebuffer(0),
	name(name),
	inited(false),
	framebuffer_id(0),
	texture_id(0)
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
		LOG_DEBUG("Fullscreen post-processor successfully built");
	} else {
		LOG_DEBUG("Failed to build fullscreen post-processor");
	}

	restoreState();
}

FullscreenShader::~FullscreenShader() {

	if(!fragment_shader.empty()) {
		LiveReloadManager::Instance()->removeLiveReloadEntry(fragment_shader.c_str(), static_cast<IReloadableFile*>(this));
	}
	inited = false;
	texture_id = 0;
	_id = 0;
	framebuffer_id = 0;

}

FullscreenShader::FullscreenShader(const FullscreenShader& other)
	:GLProgram(Array<ShaderFile>{
		{"assets/shaders/fullScreenShader.vert", GL_VERTEX_SHADER, 0},
	}),
	fragment_shader(other.fragment_shader),
	texture_inputs(other.texture_inputs),
	frame_width(other.frame_width),
	frame_height(other.frame_height),
	current_bound_program(0),
	current_bound_texture(0),
	current_bound_framebuffer(0),
	name(other.name),
	framebuffer_id(0),
	texture_id(0),
	inited(false)
{

	saveState();

	if(other.framebuffer_id == 0) {
		glGenFramebuffers(1, &framebuffer_id);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
	} else {
		framebuffer_id = other.framebuffer_id;
	}


	if(other.texture_id == 0) {

		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame_width, frame_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	} else {
		texture_id = other.texture_id;
	}

	if(other.texture_id == 0)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

	if(other.framebuffer_id == 0) {
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
			LOG_DEBUG("Fullscreen post-processor successfully built");
		} else {
			LOG_DEBUG("Failed to build fullscreen post-processor");
		}
	}

	restoreState();

	if(!fragment_shader.empty()) {
		ChangeShader(fragment_shader);
		LiveReloadManager::Instance()->addLiveReloadEntry(fragment_shader.c_str(), static_cast<IReloadableFile*>(this));
		inited = true;
	}
}


FullscreenShader::FullscreenShader(const String shader_src, Array<TextureInput> texture_inputs, u32 frame_width, u32 frame_height, const char *name)
	:GLProgram(Array<ShaderFile>{
		{"assets/shaders/fullScreenShader.vert", GL_VERTEX_SHADER, 0},
		{shader_src, GL_FRAGMENT_SHADER, 0}
	}),
	fragment_shader(shader_src),
	texture_inputs(texture_inputs),
	frame_width(frame_width),
	frame_height(frame_height),
	current_bound_program(0),
	current_bound_texture(0),
	current_bound_framebuffer(0),
	name(name)
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
	inited = true;

	IReloadableFile *x = static_cast<IReloadableFile*>(this);
	LiveReloadManager::Instance()->addLiveReloadEntry(fragment_shader.c_str(), static_cast<IReloadableFile*>(this));
}


void FullscreenShader::AddInput(u32 texture_id, u32 texture_location) {
	texture_inputs.push_back({texture_id, texture_location});
}

void FullscreenShader::RemoveInput(i32 input_index) {
	texture_inputs.erase(texture_inputs.begin() + input_index);
}

void FullscreenShader::ChangeShader(String& shader_path) {
	u32 shader = compileShader(shader_path, GL_FRAGMENT_SHADER);
	u32 shaders_attached[2];


	glGetAttachedShaders(_id, 2, NULL, shaders_attached);

	i32 shader_type = 0;
	for(u32 i = 0; i < 2; i++) {
		glGetShaderiv(shaders_attached[i], GL_SHADER_TYPE, &shader_type);
		if(shader_type == GL_FRAGMENT_SHADER) {
			glDetachShader(_id, shaders_attached[i]);
			glDeleteShader(shaders_attached[i]);
			break;
		}
	}

	glAttachShader(_id, shader);
	glLinkProgram(_id);

	// Check the program
	i32 result;
	i32 length; 
	glGetProgramiv(_id, GL_LINK_STATUS, &result);
	if (result != GL_TRUE) {
		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);
		char *buffer = (char*)StackMemAllocator::Instance()->allocate(length);
		glGetProgramInfoLog(_id, length, NULL, buffer);
		LOG_DEBUG("Error Linking program: \n%s", buffer);
		StackMemAllocator::Instance()->free((u8*)buffer);
	} else {
		if(shader_path != fragment_shader) {
			LiveReloadManager::Instance()->removeLiveReloadEntry(fragment_shader.c_str(), static_cast<IReloadableFile*>(this));
			fragment_shader = shader_path;
			LiveReloadManager::Instance()->addLiveReloadEntry(fragment_shader.c_str(), static_cast<IReloadableFile*>(this));
		}

		inited = true;
	}

}


void FullscreenShader::liveReloadFile(const char* filename, LiveReloadEntry *entry) {
	String f(filename);
	ChangeShader(f);
	entry->changed = 0;
}


void FullscreenShader::saveState() {
	// glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &current_bound_texture);
	// glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &current_bound_framebuffer);
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_bound_program);
}

void FullscreenShader::restoreState() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(current_bound_program);

}

void FullscreenShader::draw(time_type deltaTime) {


	if(!inited) return;

	saveState();
	use();

	setFloat("dt", deltaTime);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);

	for(u32 i = 0; i < texture_inputs.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + texture_inputs[i].texture_location);
		glBindTexture(GL_TEXTURE_2D, texture_inputs[i].texture_id);
	}

	// the vertex shader is common across all fullscreen shaders
	// it just contains the coords for a quad and here it renders that quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	restoreState();
}

