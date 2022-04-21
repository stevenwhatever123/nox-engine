#include "EngineGUI/AnimationPanel.h"

// hacky solution to min and max being redefined in Windows.h, causing std::min/std::max
// to give errors. See https://stackoverflow.com/questions/2561368/illegal-token-on-right-side-of
#include <limits>
#define NOMINMAX

#include <Windows.h>
#include <FBXFileLoader.h>
#include <EventManager.h>
#include <EventNames.h>

using namespace NoxEngine;

void NoxEngineGUI::updateAnimationPanel(NoxEngine::GameState *game_state) {

	// Variables
	std::string name = PANEL_NAME_MAP[ PanelName::AnimationSettings ];
	bool isAlreadyLoaded = false;

	// Window Begin
	ImGui::Begin(name.c_str());


	if (ImGui::Button("Load FBX File")) {
		String picked_file = IOManager::Instance()->PickFile("All Files\0*.*\0\0");
		if (picked_file.length() > 0)
		{
			SIGNAL_EVENT(EventNames::meshAdded, picked_file);
		}
	}


	// Window End
	ImGui::End();
}
