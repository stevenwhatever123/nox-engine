#pragma once
#include <fstream>
#include <string>

#include <Core/Types.h>
#include <Core/Entity.h>
#include <Core/GameState.h>
#include <Utils/FBXFileLoader.h>
#include <Components/TransformComponent.h>
#include <Components/RenderableComponent.h>
#include <Components/AnimationComponent.h>

namespace NoxEngine
{
	void saveScene(std::string file_path, NoxEngine::GameState& game_state);
	void loadScene(std::string file_path, NoxEngine::GameState& game_state);
}