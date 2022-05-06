#pragma once

#include <IComponent.h>
#include <Types.h>
#include <lua.hpp>

namespace NoxEngine {
	/*
	 * A mock up class for Renderer to render a cube
	 */
	class ScriptComponent : public IComponent {
		public:
			ScriptComponent(const char *script);
			ScriptComponent();

			void setScript(const char *script_name);
			void tick(time_type dt);

			void attachedToEntity(Entity*) override; 
			Entity* getParentEntity() override { return parent;};
			bool inited;


		private:
			String script_file;
			lua_State *script_state;

	};
}
