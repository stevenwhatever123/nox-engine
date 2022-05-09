#include <Components/ScriptComponent.h>
#include <Components/PositionComponent.h>

#include <Utils/Utils.h>
#include <LuaBridge/LuaBridge.h>

using namespace NoxEngine;
using luabridge::LuaRef;

ScriptComponent::ScriptComponent() {
	id = ComponentType::ScriptType;
}

ScriptComponent::ScriptComponent(const char *script): script_file(script), inited(false)  {
	id = ComponentType::ScriptType;

	script_state = luaL_newstate();
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
}

void ScriptComponent::tick(time_type dt) {

	if(inited) {
		lua_getglobal(script_state, "tick");
		lua_pushnumber(script_state, dt);
		lua_pcall(script_state, 1, 0, 0);
	}

}

void ScriptComponent::attachedToEntity(Entity *ent) {
	parent = ent;
	luaL_dofile(script_state, script_file.c_str());
	luabridge::setGlobal(script_state, parent, "self");
	inited = true;
}
