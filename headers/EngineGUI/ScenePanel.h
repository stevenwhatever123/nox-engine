#pragma once

// Core GUI
#include "EngineGUI.h"

#include <EngineGUI/ImGuizmoTool.h>

#include <Core/Renderer.h>
#include <Core/GameState.h>

#include "EngineGUI/PresetObject.h"


using NoxEngineGUI::GUIParams;
using NoxEngine::GameState;

namespace NoxEngineGUI {
	void updateScenePanel(GameState* game_state, GUIParams* params);
};
