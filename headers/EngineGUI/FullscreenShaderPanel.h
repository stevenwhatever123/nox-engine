#pragma once

//Skybox
#include <Core/GameState.h>
#include <EngineGUI/EngineGUI.h>

namespace NoxEngineGUI {
	void updateFullscreenShaderPanel(NoxEngine::GameState* game_state, GUIParams *params);
	void updatePostProcessorsPanel(NoxEngine::GameState* game_state, GUIParams *params);
};

