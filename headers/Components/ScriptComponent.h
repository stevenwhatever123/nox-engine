#pragma once

#include "IComponent.h"
#include <Core/Types.h>
#include <3rdParty/lua/lua.hpp>
#include <Managers/IReloadableFile.h>

namespace NoxEngine {
	class ScriptComponent : public IComponent, public IReloadableFile {
		public:
			static const ComponentType id = ComponentType::ScriptType;

			ScriptComponent(const char *script);
			ScriptComponent();
			~ScriptComponent();

			const char* getScript() const;
			void setScript(const char *script_name);
			void tick(time_type dt, time_type currentTime);
			void liveReloadFile(const char *filename, LiveReloadEntry *entry) override;

			void attachedToEntity(Entity*) override; 
			Entity* getParentEntity() override { return parent;};
			bool inited;


		private:
			String script_file;
			lua_State *script_state;

	};
}
