#include "ScriptsManager.h"
#include "GameManager.h"
#include "Mesh.h"
#include "PositionComponent.h"

namespace NoxEngine
{
	void ScriptsManager::Init()
	{
		luaStateMachine = luaL_newstate();
		luaL_openlibs(luaStateMachine);
		Entity::exportLua();
		// GameManager::exportLua();
		Mesh::exportLua();
		PositionComponent::exportLua();
	}
	void ScriptsManager::Updcate()
	{
	}
	void ScriptsManager::Destroy()
	{
	}
	void ScriptsManager::DoLuaFile(std::string path)
	{
		luaL_openlibs(luaStateMachine);
		luaL_dofile(luaStateMachine, path.c_str());
		lua_pcall(luaStateMachine, 0, 0, 0);
	}
	lua_State* ScriptsManager::get_lua_state()
	{
		return luaStateMachine;
	}
};