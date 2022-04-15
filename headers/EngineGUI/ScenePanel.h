#pragma once

// Core GUI
#include "EngineGUI.h"

// Rendering classes
#include "Renderer.h"
#include <GameState.h>

using NoxEngine::GameState;
namespace NoxEngineGUI {
	void updateScenePanel(GameState* game_state);
};
