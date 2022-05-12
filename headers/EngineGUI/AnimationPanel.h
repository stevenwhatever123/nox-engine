#pragma once

// Core GUI
#include "EngineGUI.h"

// Animation classes
// TODO: Add here

// Other classes
#include "Core/Renderer.h"
#include "Core/MeshScene.h"
#include <Core/GameState.h>
#include <Managers/IOManager.h>
#include <Core/Entity.h>

#include <EngineGUI/AnimationSequencer.h>

namespace NoxEngineGUI {

	static AnimationSequencer sequencer;

	void initAnimationPanel();
	void updateAnimationPanel(NoxEngine::GameState *game_state, GUIParams* params);
	void updateSequencer(NoxEngine::GameState* game_state, NoxEngine::AnimationComponent* animComp);
};
