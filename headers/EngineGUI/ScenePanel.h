#pragma once

// Core GUI
#include "EngineGUI.h"

#include <EngineGUI/ImGuizmoTool.h>

#include <Core/Renderer.h>
#include <Core/GameState.h>

#include <Components/CameraComponent.h>

#include "EngineGUI/PresetObject.h"


using NoxEngineGUI::GUIParams;
using NoxEngine::GameState;

namespace NoxEngineGUI {
	void updateScenePanel(GameState* game_state, GUIParams *ui_params);
	glm::vec3 getPosOfMouseInWorldCoord(GameState* params, f32 pointX, f32 pointY, f32 width, f32 height);
};
