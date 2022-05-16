#pragma once
/*
 * An interface: comunication with Renderer system of the engine.
 * Allows a class that implements this interface to be treated as light emitter
 */
#pragma once

#include <Core/Types.h>

namespace NoxEngine {

	class IEmission
	{
	public:

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;


		virtual glm::vec3 get_ambient() const { return ambient; }
		virtual glm::vec3 get_diffuse() const { return diffuse; }
		virtual glm::vec3 get_specular() const { return specular; }

		virtual void set_ambient(glm::vec3 value) { ambient = value; }
		virtual void set_diffuse(glm::vec3 value) { diffuse = value; }
		virtual void set_specular(glm::vec3 value) { specular = value; }
	};
};