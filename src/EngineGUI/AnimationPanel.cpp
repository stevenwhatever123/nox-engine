#include "EngineGUI/AnimationPanel.h"

// hacky solution to min and max being redefined in Windows.h, causing std::min/std::max
// to give errors. See https://stackoverflow.com/questions/2561368/illegal-token-on-right-side-of
#include <limits>
#define NOMINMAX

#include <Windows.h>
#include "../FBXFileLoader.h"


namespace NoxEngine {

	namespace EngineGUI {

		void updateAnimationPanel(const GUIParams* params) {

			// Variables
			std::string name = PANEL_NAME_MAP[ PanelName::AnimationSettings ];
			Mesh* mesh = nullptr;
			bool isAlreadyLoaded = false;

			// Window Begin
			ImGui::Begin(name.c_str());


			/*   Logic   */
			if (ImGui::Button("Load FBX File")) {
				OPENFILENAME ofn;

				wchar_t szFile[256];

				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = NULL;
				ofn.lpstrFile = szFile;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = L"Filmbox File (*.fbx)\0*.fbx\0All (*.*)\0*.*\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.lpstrTitle = L"Select FBX file";
				ofn.lpstrDefExt = L"fbx";
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				bool readSuccess = GetOpenFileName(&ofn);

				// LPWSTR is different from char
				// Therefore we're converting it
				char filenameBuffer[256];
				wcstombs_s(nullptr, filenameBuffer, sizeof(filenameBuffer), szFile, sizeof(szFile));

				if (readSuccess)
				{
					printf("Read Success\n");

					const aiScene* pScene = FBXFileLoader::readFBX(filenameBuffer);

					// Converting the scene to mesh data
					if (pScene != nullptr)
					{
						mesh = FBXFileLoader::getMesh(pScene);
						isAlreadyLoaded = true;
						//curMesh = mesh;
					}
					else
					{
						printf("Error: converting sence to mesh data");
					}
				}
				else
				{
					printf("Error: reading fbx file");
				}
			}


			// If there was a mesh loaded by user
			// TODO: Might be a good idea to move this to ScenePanel?
			//       Have a queue of loaded meshes then add/update in ScenePanel if it's not empty
			if (isAlreadyLoaded) {
				// Add mesh to renderer
				mesh->prepForRenderer();
				params->renderer->addObject(mesh);
				params->renderer->updateBuffers();
				isAlreadyLoaded = false;
			}


			// Window End
			ImGui::End();
		}
	}
}