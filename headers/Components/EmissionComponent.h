#pragma once

#include "IComponent.h"
#include "IEmission.h"
#include <Core/Types.h>
#include <Core/Entity.h>

/*
*  Emission Component
*/

namespace NoxEngine {

	class EmissionComponent : public IComponent, public IEmission
	{
	public:
		EmissionComponent(glm::vec3 amb = glm::vec3(1.0f), glm::vec3 diff = glm::vec3(1.0f), glm::vec3 spec = glm::vec3(1.0f))
		{
			ambient = amb; diffuse = diff; specular = spec;
		}

		static const ComponentType id = ComponentType::LightSourceType;

		static void exportLua() { };


		glm::vec3 get_ambient() const override { return ambient; }
		glm::vec3 get_diffuse() const override { return diffuse; }
		glm::vec3 get_specular() const override { return specular; }

		void set_ambient(glm::vec3 value) override { ambient = value; }
		void set_diffuse(glm::vec3 value) override { diffuse = value; }
		void set_specular(glm::vec3 value) override { specular = value; }


		Entity* getParentEntity() override { return parent; };
		void attachedToEntity(Entity* ent) override { parent = ent; };
	};
};