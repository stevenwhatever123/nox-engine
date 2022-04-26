#define GLEW_STATIC
#define _CRT_SECURE_NO_WARNINGS

// Engine Include
#include "GameManager.h"
using NoxEngine::GameManager;

int main(int argc, char** argv) {
	GameManager gm(1920, 1080, "Nox Engine");
	gm.init();
	while(gm.KeepRunning()) {
		gm.update();
	}
	return 0;
}

