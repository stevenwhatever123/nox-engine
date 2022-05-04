#pragma once

#include <EngineGUI/EngineGUI.h>
#include "EngineGUI/AnimationPanel.h"
#include "EngineGUI/AudioPanel.h"
#include "EngineGUI/ScenePanel.h"
#include "EngineGUI/PresetObjectPanel.h"
#include "EngineGUI/ImGuizmoTool.h"

#include <fstream>

using namespace NoxEngine;

void NoxEngineGUI::init_imgui(GLFWwindow* win) {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	// Config flags
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // All subsequent windows have Docking behaviour
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	io.ConfigFlags |= ImGuiWindowFlags_NoMove;					// "Make sure the main window cannot move" - doesn't work?

	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(win, true);
	ImGui_ImplOpenGL3_Init("#version 450");
}

// Make a window as big as the viewport
// This is expected to run ONCE, AFTER an ImGui::NewFrame is created
void NoxEngineGUI::setupFixedLayout() {

	//ImGuiDockNode* mainNode = nullptr;
	ImGuiID mainNodeID = ImGui::GetID("MainDockNode");

	ImGui::DockBuilderRemoveNode(mainNodeID);	// remove the node from the saved imgui.ini file from a previous run
	ImGui::DockBuilderAddNode(mainNodeID, ImGuiDockNodeFlags_None);

	// Make the dock node's size and position to match the viewport
	ImGui::DockBuilderSetNodeSize(mainNodeID, ImGui::GetMainViewport()->WorkSize);
	ImGui::DockBuilderSetNodePos(mainNodeID, ImGui::GetMainViewport()->WorkPos);

	// Design the layout here with DockBuilderSplitNode
	ImGuiID dock_right_id		= ImGui::DockBuilderSplitNode(mainNodeID, ImGuiDir_Right, 0.2f, nullptr, &mainNodeID);
	ImGuiID dock_left_id		= ImGui::DockBuilderSplitNode(mainNodeID, ImGuiDir_Left, 0.2f, nullptr, &mainNodeID);
	ImGuiID dock_left_down_id	= ImGui::DockBuilderSplitNode(dock_left_id, ImGuiDir_Down, 0.5f, nullptr, &dock_left_id);
	ImGuiID dock_down_id		= ImGui::DockBuilderSplitNode(mainNodeID, ImGuiDir_Down, 0.2f, nullptr, &mainNodeID);
	ImGuiID dock_down_right_id	= ImGui::DockBuilderSplitNode(dock_down_id, ImGuiDir_Right, 0.4f, nullptr, &dock_down_id);
	ImGuiID dock_down_down_id	= ImGui::DockBuilderSplitNode(mainNodeID, ImGuiDir_Up, 0.05f, nullptr, &mainNodeID);

	ImGui::DockBuilderDockWindow(PANEL_NAME_MAP[PanelName::Scene].c_str(), mainNodeID);
	ImGui::DockBuilderDockWindow(PANEL_NAME_MAP[PanelName::Inspector].c_str(), dock_right_id);
	ImGui::DockBuilderDockWindow(PANEL_NAME_MAP[PanelName::PresetObjects].c_str(), dock_left_id);
	ImGui::DockBuilderDockWindow(PANEL_NAME_MAP[PanelName::Hierarchy].c_str(), dock_left_down_id);
	ImGui::DockBuilderDockWindow(PANEL_NAME_MAP[PanelName::AnimationSettings].c_str(), dock_down_id);
	ImGui::DockBuilderDockWindow(PANEL_NAME_MAP[PanelName::AudioSource].c_str(), dock_down_right_id);
	ImGui::DockBuilderDockWindow(PANEL_NAME_MAP[PanelName::FileExplorer].c_str(), dock_down_down_id);
			
	// Change node flags here
#if 0
	mainNode = ImGui::DockBuilderGetNode(dock_main_id);

	// recursively go up to get the root node
	while (mainNode->ParentNode != nullptr) {
		mainNode = mainNode->ParentNode;
		printf("ID    (%x)\n", mainNode->ID);
	}
#endif

	ImGui::DockBuilderFinish(mainNodeID);

}


void NoxEngineGUI::cleanupImGui() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


void NoxEngineGUI::updateMenu(NoxEngine::GameState& game_state, GUIParams *params) {

	// Create menu bar
	if (ImGui::BeginMenuBar()) {

		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Save"))
			{
				std::string file_path = "assets/blablabla.txt";

				std::ofstream outputStream(file_path, std::ios::binary);
				if (outputStream.is_open())
				{
					size_t numOfEntities = game_state.activeScene->entities.size();
					outputStream.write((char*)&numOfEntities, sizeof(size_t));

					for (Entity* ent : game_state.activeScene->entities)
					{
						// Getting the name
						std::string entityName(ent->name);
						size_t entityNameSize = entityName.size();
						outputStream.write((char*)&entityNameSize, sizeof(entityNameSize));
						outputStream.write((char*)entityName.c_str(), entityNameSize);

						// Get the file path if available
						bool loadedFromFile = ent->filepath != nullptr;
						outputStream.write((char*)&loadedFromFile, sizeof(bool));
						if (loadedFromFile)
						{
							std::string filepath(ent->filepath);
							size_t filepathSize = filepath.size();
							outputStream.write((char*)&filepathSize, sizeof(filepathSize));
							outputStream.write((char*)filepath.c_str(), filepathSize);
						}

						// Check if this entity have any component;
						bool hasComponent = ent->hasComp;
						outputStream.write((char*)&hasComponent, sizeof(bool));

						// If has component, write the component one by one
						if (hasComponent)
						{
							// Starting with position first
							bool hasTransform = ent->containsComps(TransformFlag);
							outputStream.write((char*)&hasTransform, sizeof(bool));

							if (hasTransform)
							{
								ITransform* transform = ent->getComp<TransformComponent>()->CastType<ITransform>();

								outputStream.write((char*)&transform->x, sizeof(f32));
								outputStream.write((char*)&transform->y, sizeof(f32));
								outputStream.write((char*)&transform->z, sizeof(f32));

								outputStream.write((char*)&transform->rx, sizeof(f32));
								outputStream.write((char*)&transform->ry, sizeof(f32));
								outputStream.write((char*)&transform->rz, sizeof(f32));

								outputStream.write((char*)&transform->sx, sizeof(f32));
								outputStream.write((char*)&transform->sy, sizeof(f32));
								outputStream.write((char*)&transform->sz, sizeof(f32));
							}

							// Renderable Component
							bool hasRenderableComp = ent->containsComps(RenderableFlag);
							outputStream.write((char*)&hasRenderableComp, sizeof(bool));

							if (hasRenderableComp)
							{
								if (loadedFromFile)
								{
									RenderableComponent* rendComp = ent->getComp<RenderableComponent>();

									// Pair using the mesh name associated with this entity
									Mesh2 *mesh = (Mesh2*) rendComp;
									std::string meshName(mesh->name);
									size_t meshNameSize = meshName.size();
									outputStream.write((char*)&meshNameSize, sizeof(meshNameSize));
									outputStream.write((char*)meshName.c_str(), meshNameSize);

									std::string ambientTexture(rendComp->getAmbientTexture());
									size_t ambientTextureSize = ambientTexture.size();

									outputStream.write((char*)&ambientTextureSize, sizeof(ambientTextureSize));
									outputStream.write((char*)ambientTexture.c_str(), ambientTextureSize);
								}

							}
						}
					}

					outputStream.close();
				}

			}

			if (ImGui::MenuItem("Load"))
			{
				std::string file_path = IOManager::Instance()->PickFile();

				std::ifstream inputStream(file_path, std::ios::binary);
				if (inputStream.is_open())
				{
					size_t numOfEntities;
					inputStream.read((char*)&numOfEntities, sizeof(size_t));

					for (u32 i = 0; i < numOfEntities; i++)
					{
						// Get the name
						std::string entityName;
						size_t entityNameSize;
						inputStream.read((char*)&entityNameSize, sizeof(entityNameSize));
						entityName.resize(entityNameSize);
						inputStream.read((char*)&entityName[0], entityNameSize);

						Entity* ent = nullptr;

						// Is it loaded from a file?
						bool loadedFromFile;
						inputStream.read((char*)&loadedFromFile, sizeof(bool));
						std::string fbx_filepath;
						if (loadedFromFile)
						{
							size_t fbx_filepathSize;
							inputStream.read((char*)&fbx_filepathSize, sizeof(fbx_filepathSize));
							fbx_filepath.resize(fbx_filepathSize);
							inputStream.read((char*)&fbx_filepath[0], fbx_filepathSize);

							ent = new Entity(game_state.activeScene, entityName.c_str(), fbx_filepath.c_str());
						}
						else
						{
							ent = new Entity(game_state.activeScene, entityName.c_str());
						}

						// Read component if available
						bool hasComponent;
						inputStream.read((char*)&hasComponent, sizeof(bool));

						if (hasComponent)
						{
							// Does it has a transform?
							bool hasTransform;
							inputStream.read((char*)&hasTransform, sizeof(bool));

							if (hasTransform)
							{
								TransformComponent* pos = new TransformComponent(0.0, 0.0, 0.0);

								inputStream.read((char*)&pos->x, sizeof(f32));
								inputStream.read((char*)&pos->y, sizeof(f32));
								inputStream.read((char*)&pos->z, sizeof(f32));

								inputStream.read((char*)&pos->rx, sizeof(f32));
								inputStream.read((char*)&pos->ry, sizeof(f32));
								inputStream.read((char*)&pos->rz, sizeof(f32));

								inputStream.read((char*)&pos->sx, sizeof(f32));
								inputStream.read((char*)&pos->sy, sizeof(f32));
								inputStream.read((char*)&pos->sz, sizeof(f32));

								ent->addComp(pos);
							}

							// Does it has renderable comp?
							bool hasRenderableComp;
							inputStream.read((char*)&hasRenderableComp, sizeof(bool));

							if (hasRenderableComp)
							{
								if (loadedFromFile)
								{
									game_state.meshScenes.emplace(fbx_filepath, NoxEngine::readFBX(fbx_filepath.c_str()));
									MeshScene& meshScene = game_state.meshScenes.find(fbx_filepath)->second;

									std::string meshName;
									size_t meshNameSize;
									inputStream.read((char*)&meshNameSize, sizeof(meshNameSize));
									meshName.resize(meshNameSize);
									inputStream.read((char*)&meshName[0], meshNameSize);

									RenderableComponent* rendComp = nullptr;

									for (Mesh2* mesh : meshScene.meshes)
									{
										if(strcmp(mesh->name.c_str(), meshName.c_str()) == 0)
										{
											rendComp = mesh;
											break;
										}
									}

									std::string ambientTexture;
									size_t ambientTextureSize;
									inputStream.read((char*)&ambientTextureSize, sizeof(ambientTextureSize));
									ambientTexture.resize(ambientTextureSize);
									inputStream.read((char*)&ambientTexture[0], ambientTextureSize);
									rendComp->ambientTexture = ambientTexture;

									ent->addComp(rendComp);
								}
							}
						}

						game_state.activeScene->addEntity(ent);
					}

					inputStream.close();
				}
			}

			if (ImGui::MenuItem("Close", NULL, false, p_open != NULL)) *p_open = false;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit")) {
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Preferences")) {


			static float backColor[] = HEX_TO_FLOAT4(params->sceneBackgroundColor);
			ImGui::ColorPicker4("Scene Background", backColor);
			params->sceneBackgroundColor = FLOAT4_TO_HEX(backColor);
			// if (ImGui::MenuItem("Background Color", NULL, false, p_open != NULL)){
				
			// }

			if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}


void NoxEngineGUI::updateGUI(NoxEngine::GameState& game_state,GUIParams *params) {

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar
		         | ImGuiWindowFlags_NoCollapse
				 | ImGuiWindowFlags_NoResize
				 | ImGuiWindowFlags_NoMove
				 | ImGuiWindowFlags_NoBringToFrontOnFocus
				 | ImGuiWindowFlags_NoNavFocus
				 | ImGuiWindowFlags_MenuBar
				 | ImGuiWindowFlags_NoDocking;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::Begin("Invisible Window", p_open, window_flags);
	ImGui::PopStyleVar(2);

	// Menu bar
	updateMenu(game_state, params);

	// Setup dockspace only once
	if (params->firstLoop) setupFixedLayout();

	// Submit the DockSpace
	// Note: Uncomment this to allow the entire viewport to be dockable
	//       e.g. you can maximize a window
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspace_id = ImGui::GetID("MainDockNode");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();	// End to "Invisible Window"


	// Placeholder / debug windows
	ImGui::Begin(PANEL_NAME_MAP[PanelName::FileExplorer].c_str());    

	ImGui::SliderFloat3("Camera ", glm::value_ptr(params->current_cam->user_shift), 0.0f, 100.0f);
	ImGui::End();


	// First loop is over
	params->firstLoop = false;
}
