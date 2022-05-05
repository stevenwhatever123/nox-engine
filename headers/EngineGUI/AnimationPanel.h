#pragma once

// Core GUI
#include "EngineGUI.h"

// Animation classes
// TODO: Add here

// Other classes
#include "Renderer.h"
#include "MeshScene.h"
#include <GameState.h>
#include <IOManager.h>
#include <Entity.h>

#include <EngineGUI/AnimationSequencer.h>

namespace NoxEngineGUI {

	static AnimationSequencer sequencer;

	void initAnimationPanel();
	void updateAnimationPanel(NoxEngine::GameState *game_state, GUIParams* params);
	void updateSequencer(NoxEngine::Entity *ent, NoxEngine::GameState* game_state);
};
