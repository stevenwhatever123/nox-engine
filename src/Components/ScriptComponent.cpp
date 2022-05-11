#include <Components/ScriptComponent.h>
#include <Components/TransformComponent.h>

#include <Utils/Utils.h>
#include <LuaBridge/LuaBridge.h>

using namespace NoxEngine;
using luabridge::LuaRef;

ScriptComponent::ScriptComponent() {
	//id = ComponentType::ScriptType;
}

ScriptComponent::ScriptComponent(const char *script): script_file(script), inited(false)  {
	//id = ComponentType::ScriptType;

	script_state = luaL_newstate();
	luaL_openlibs(script_state);

	luabridge::getGlobalNamespace(script_state)
		.beginNamespace("game")

		.beginClass<Entity>("Entity")
			.addProperty("id", &Entity::id)
			.addFunction("GetTransformComponent", &Entity::getComp<TransformComponent>)
		.endClass()

		.beginClass<TransformComponent>("TransformComponent")
		.addProperty("x", &TransformComponent::get_x, &TransformComponent::set_x)
		.addProperty("y", &TransformComponent::get_y, &TransformComponent::set_y)
		.addProperty("z", &TransformComponent::get_z, &TransformComponent::set_z)

		.addProperty("rx", &TransformComponent::get_rx, &TransformComponent::set_rx)
		.addProperty("ry", &TransformComponent::get_ry, &TransformComponent::set_ry)
		.addProperty("rz", &TransformComponent::get_rz, &TransformComponent::set_rz)

		.addProperty("sx", &TransformComponent::get_sx, &TransformComponent::set_sx)
		.addProperty("sy", &TransformComponent::get_sy, &TransformComponent::set_sy)
		.addProperty("sz", &TransformComponent::get_sz, &TransformComponent::set_sz)
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
