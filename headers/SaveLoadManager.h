#pragma once
#include <fstream>
#include <string>

#include <Types.h>
#include <Entity.h>
#include <GameState.h>
#include <FBXFileLoader.h>
#include <TransformComponent.h>
#include <RenderableComponent.h>
#include <AnimationComponent.h>

namespace NoxEngine
{
	void saveScene(std::string file_path, NoxEngine::GameState& game_state);
	void loadScene(std::string file_path, NoxEngine::GameState& game_state);
}