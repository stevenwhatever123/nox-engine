#pragma once

// Core GUI
#include "EngineGUI.h"
#include <Core/Renderer.h>

#include <Core/GameState.h>
#include "EngineGUI/PresetObject.h"


using NoxEngine::GameState;
namespace NoxEngineGUI {
	void updateScenePanel(GameState* game_state);
};
