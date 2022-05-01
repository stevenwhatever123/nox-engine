#pragma once
#include <Singleton.h>
#include <Types.h>
#include <iostream>
extern "C" {
# include "lua/lua.h"
# include "lua/lauxlib.h"
# include "lua/lualib.h"
}
#include <LuaBridge/LuaBridge.h>
#include <string>
namespace NoxEngine
{
	class ScriptsManager : public Singleton<ScriptsManager> {
		friend class Singleton<ScriptsManager>;
		

	private:
		lua_State* luaStateMachine;

	protected:
		ScriptsManager() {};
		~ScriptsManager() {};

	public:
		// dummy constructors and destroyers

		// Initialize
		void Init();

		// update loop
		void Updcate();

		// shutdown fmod and free all resources
		void Destroy();

		void DoLuaFile(std::string path);

		lua_State* get_lua_state();




	};
};
