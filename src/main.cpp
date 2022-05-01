#define GLEW_STATIC
#define _CRT_SECURE_NO_WARNINGS

// System/std includes
#include <Windows.h>

// Engine Include
#include <GameManager.h>
#include "script.h"
using NoxEngine::GameManager;

int main(int argc, char** argv) {

#if 1

	GameManager gm(1280, 720, "Nox Engine");
	gm.init();

	while(gm.KeepRunning()) {


		gm.update();

	}
#endif
	/*auto a = new LuaTest();
	a->InitLua();
	a->Test()*/;
	return 0;
}

