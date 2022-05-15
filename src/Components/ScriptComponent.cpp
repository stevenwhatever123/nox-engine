#include <Components/ScriptComponent.h>
#include <Components/TransformComponent.h>
#include <Components/CameraComponent.h>

#include <Utils/Utils.h>
#include <LuaBridge/LuaBridge.h>
#include <Managers/LiveReloadManager.h>

using namespace NoxEngine;
using luabridge::LuaRef;

ScriptComponent::ScriptComponent() {
	//id = ComponentType::ScriptType;
	script_state = luaL_newstate();
	luaL_openlibs(script_state);
	luabridge::getGlobalNamespace(script_state)
		.beginNamespace("game")

		.beginClass<Entity>("Entity")
			.addProperty("id", &Entity::id)
			.addFunction("GetPositionComponent", &Entity::getComp<TransformComponent>)
			.addFunction("GetCameraComponent", &Entity::getComp<CameraComponent>)
		.endClass()

		.beginClass<vec3>("vec3")
			.addProperty("x", &vec3::x, &vec3::x)
			.addProperty("y", &vec3::y, &vec3::y)
			.addProperty("z", &vec3::z, &vec3::z)
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

		.beginClass<Camera>("Camera")
			.addFunction("pos", &Camera::GetCameraPosition)
			.addFunction("forward", &Camera::GetCameraForward)
			.addFunction("up", &Camera::GetCameraUp)

			.addProperty("yaw", &Camera::getYaw, &Camera::setYaw)
			.addProperty("pitch", &Camera::getPitch, &Camera::setPitch)
		.endClass()

		.beginClass<CameraComponent>("CameraComponent")
			.addFunction("camera", &CameraComponent::getCamera)
		.endClass()


		.endNamespace();
}

ScriptComponent::~ScriptComponent() {
	LiveReloadManager::Instance()->removeLiveReloadEntry(script_file.c_str(), static_cast<IReloadableFile*>(this));

	lua_close(script_state);
	inited = false;

}

ScriptComponent::ScriptComponent(const char *script): ScriptComponent() {
	script_file = script;
	LiveReloadManager::Instance()->addLiveReloadEntry(script_file.c_str(), static_cast<IReloadableFile*>(this));
}

void ScriptComponent::setScript(const char *script) {

	if(parent != NULL) {

		LiveReloadManager::Instance()->removeLiveReloadEntry(script_file.c_str(), static_cast<IReloadableFile*>(this));

		script_file = script;

		luaL_dofile(script_state, script_file.c_str());
		luabridge::setGlobal(script_state, parent, "self");

		LiveReloadManager::Instance()->addLiveReloadEntry(script_file.c_str(), static_cast<IReloadableFile*>(this));

		inited = true;
		
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

	if(!script_file.empty()) {
		luaL_dofile(script_state, script_file.c_str());
		luabridge::setGlobal(script_state, parent, "self");
		inited = true;
	}
}

void ScriptComponent::liveReloadFile(const char *file_name, LiveReloadEntry *entry) {

	luaL_dofile(script_state, script_file.c_str());
	luabridge::setGlobal(script_state, parent, "self");
	entry->changed = 0;

}


