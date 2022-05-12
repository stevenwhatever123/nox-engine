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
		{0.0f, 0.0f, 10.0f},		// Forward
		{0.0f, 3.0f, 7.0f},		// up
		{0.0f, -3.0f, 7.0f},	// down
		{-3.0f, 0.0f, 7.0f},	// left
		{3.0f, 0.0f, 7.0f}		// right
	};

	indices = {
		0, 1,
		1, 2,
		1, 3,
		1, 4,
		1, 5
	};
}