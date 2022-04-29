#define _CRT_SECURE_NO_WARNINGS

// Engine Include
#include "GameManager.h"
using NoxEngine::GameManager;

int main(int argc, char** argv) {

	GameManager *gm = GameManager::Instance();
	gm->init();
	while(gm->KeepRunning()) {
		gm->update();
	}
	return 0;
}

