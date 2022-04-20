#define GLEW_STATIC
#define _CRT_SECURE_NO_WARNINGS

// System/std includes
#include <Windows.h>

// Engine Include
#include <GameManager.h>
using NoxEngine::GameManager;

int main(int argc, char** argv) {

	GameManager gm(1280, 720, "Nox Engine");
	gm.init();
	while(gm.KeepRunning()) {
		gm.update();
	}
	return 0;
}

