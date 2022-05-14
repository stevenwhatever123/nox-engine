#include <Components/AudioListenerComponent.h>

using namespace NoxEngine;


AudioListenerComponent::AudioListenerComponent() {

	active = false;
	vVel = vec3(0.0f);

	has_texture = 0;
	has_normal = 0;
	use_indices = 1;

	glRenderType = GL_LINES;

	// TODO: Generate vertices and indices for rendering

	vertices = {
		{0.0f, 0.0f, 0.0f},		// Origin of listener
		{0.0f, 0.0f, 8.0f},		// Forward
		{0.0f, 2.0f, 6.0f},		// up
		{0.0f, -2.0f, 6.0f},	// down
		{-2.0f, 0.0f, 6.0f},	// left
		{2.0f, 0.0f, 6.0f}		// right
	};

	indices = {
		0, 1,
		1, 2,
		1, 3,
		1, 4,
		1, 5
	};
}