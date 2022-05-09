#include <Components/ScriptComponent.h>
#include <Components/PositionComponent.h>

#include <Utils/Utils.h>
#include <LuaBridge/LuaBridge.h>
#include <Managers/LiveReloadManager.h>

using namespace NoxEngine;
using luabridge::LuaRef;

ScriptComponent::ScriptComponent() {
	id = ComponentType::ScriptType;
}

ScriptComponent::ScriptComponent(const char *script): script_file(script), inited(false)  {
	id = ComponentType::ScriptType;

	script_state = luaL_newstate();

	LiveReloadManager::Instance()->addLiveReloadEntry(script_file.c_str(), static_cast<IReloadableFile*>(this));

	luaL_openlibs(script_state);

	luabridge::getGlobalNamespace(script_state)
		.beginNamespace("game")

		.beginClass<Entity>("Entity")
			.addProperty("id", &Entity::id)
			.addFunction("GetPositionComponent", &Entity::getComp<PositionComponent>)
		.endClass()

		.beginClass<PositionComponent>("PositionComponent")
			.addProperty("x", &PositionComponent::get_x, &PositionComponent::set_x)
			.addProperty("y", &PositionComponent::get_y, &PositionComponent::set_y)
			.addProperty("z", &PositionComponent::get_z, &PositionComponent::set_z)
		.endClass()

		.endNamespace();

	
}

void ScriptComponent::setScript(const char *script) {

	if(inited && parent != NULL) {


		LiveReloadManager::Instance()->removeLiveReloadEntry(script_file.c_str(), static_cast<IReloadableFile*>(this));

		script_file = script;

		luaL_dofile(script_state, script_file.c_str());
		luabridge::setGlobal(script_state, parent, "self");

		LiveReloadManager::Instance()->addLiveReloadEntry(script_file.c_str(), static_cast<IReloadableFile*>(this));
	}

}

const char* ScriptComponent::getScript() const {
	return script_file.c_str();
}


void ScriptComponent::tick(time_type dt, time_type currentTime) {

	if(inited) {

		lua_getglobal(script_state, "tick");
		lua_pushnumber(script_state, dt);
		lua_pushnumber(script_state, currentTime);
		lua_pcall(script_state, 2, 0, 0);

	}

}

void ScriptComponent::attachedToEntity(Entity *ent) {
	parent = ent;
	luaL_dofile(script_state, script_file.c_str());
	luabridge::setGlobal(script_state, parent, "self");
	inited = true;
}

void ScriptComponent::liveReloadFile(const char *file_name, LiveReloadEntry *entry) {

	luaL_dofile(script_state, script_file.c_str());
	luabridge::setGlobal(script_state, parent, "self");
	entry->changed = 0;

}
