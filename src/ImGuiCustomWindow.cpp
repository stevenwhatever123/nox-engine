#include <iostream>
#include <Windows.h>

#include "ImGuiCustomWindow.h"
#include "FBXFileLoader.h"

// Compiler is my biggest enemy
#define NOMINMAX
#include <assimp/scene.h>
#undef NOMINMAX


ImGuiCustomWindow::ImGuiCustomWindow()
{

}

ImGuiCustomWindow::~ImGuiCustomWindow()
{

}


void ImGuiCustomWindow::showFBXLoaderMenu(Mesh * curMesh)
{
	ImGui::Begin("Settings");

	if (ImGui::Button("Load FBX File"))
	{
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

			const aiScene* pScene =  FBXFileLoader::readFBX(filenameBuffer);

			// Converting the scene to mesh data
			if (pScene != nullptr)
			{
				curMesh = FBXFileLoader::getMesh(pScene);
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

	ImGui::End();
}