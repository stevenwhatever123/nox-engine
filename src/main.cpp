#define _CRT_SECURE_NO_WARNINGS

// Engine Include
#include <Singleton.h>
#include "GameManager.h"
#include <ScriptsManager.h>
#include <Utils.h>

using NoxEngine::GameManager;


int main(int argc, char** argv) {

	GameManager *gm = GameManager::Instance();

	// ScriptsManager::Instance()->Init();
	// auto lua_state = ScriptsManager::Instance()->get_lua_state();


	// luabridge::getGlobalNamespace(lua_state)
	// 	.beginNamespace("game")
	// 	.addVariable("game_manager", GameManager::Instance())
	// 	.beginClass<GameManager>("gm")
	// 	.addFunction("exportLua", &GameManager::exportLua)
	// 	.endClass()
	// 	.addFunction("run_export", get_gm)
	// 	.endNamespace();

	gm->init();
	while(gm->KeepRunning()) {
		gm->update();
	}
	return 0;
}

