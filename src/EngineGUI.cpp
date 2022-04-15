#pragma once

#include "EngineGUI.h"

#include "EngineGUI/AnimationPanel.h"
#include "EngineGUI/AudioPanel.h"
#include "EngineGUI/ScenePanel.h"
#include "EngineGUI/ImGuizmoTool.h"


namespace NoxEngine {

	namespace EngineGUI {


		void init_imgui(GLFWwindow* win) {
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


		void cleanupImGui() {

			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}


		// Make a window as big as the viewport
		// This is expected to run ONCE, AFTER an ImGui::NewFrame is created
		void setupFixedLayout() {

			//ImGuiDockNode* mainNode = nullptr;
			ImGuiID mainNodeID = ImGui::GetID("MainDockNode");

			ImGui::DockBuilderRemoveNode(mainNodeID);	// remove the node from the saved imgui.ini file from a previous run
			ImGui::DockBuilderAddNode(mainNodeID, ImGuiDockNodeFlags_None);

			// Make the dock node's size and position to match the viewport
			ImGui::DockBuilderSetNodeSize(mainNodeID, ImGui::GetMainViewport()->WorkSize);
			ImGui::DockBuilderSetNodePos(mainNodeID, ImGui::GetMainViewport()->WorkPos);

			// Design the layout here with DockBuilderSplitNode
			ImGuiID dock_right_id	= ImGui::DockBuilderSplitNode(mainNodeID, ImGuiDir_Right, 0.2f, nullptr, &mainNodeID);
			ImGuiID dock_left_id	= ImGui::DockBuilderSplitNode(mainNodeID, ImGuiDir_Left, 0.2f, nullptr, &mainNodeID);
			ImGuiID dock_down_id	= ImGui::DockBuilderSplitNode(mainNodeID, ImGuiDir_Down, 0.2f, nullptr, &mainNodeID);
			ImGuiID dock_down_right_id	= ImGui::DockBuilderSplitNode(dock_down_id, ImGuiDir_Right, 0.6f, nullptr, &dock_down_id);
			ImGuiID dock_down_down_id	= ImGui::DockBuilderSplitNode(mainNodeID, ImGuiDir_Up, 0.05f, nullptr, &mainNodeID);

			ImGui::DockBuilderDockWindow(PANEL_NAME_MAP[PanelName::Scene].c_str(), mainNodeID);
			ImGui::DockBuilderDockWindow(PANEL_NAME_MAP[PanelName::PresetObjects].c_str(), dock_right_id);
			ImGui::DockBuilderDockWindow(PANEL_NAME_MAP[PanelName::Hierarchy].c_str(), dock_left_id);
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

			// TODO-FIX: Main window is still movable!
			mainNode->LocalFlagsInWindows |= ImGuiWindowFlags_NoMove;
			mainNode->UpdateMergedFlags();

			// get the implicitly named window, set flags
			//ImGuiWindow* mainDockWindow = mainNode->HostWindow;
			//if (mainDockWindow != nullptr) {
			//	mainDockWindow->Flags |= window_flags;
			//	std::cout << "Window ID: " << mainNode->VisibleWindow->Name << "\n";
			//}
#endif

			ImGui::DockBuilderFinish(mainNodeID);

		}


		void updateMenu() {

			// Create menu bar
			if (ImGui::BeginMenuBar()) {

				if (ImGui::BeginMenu("File")) {

					if (ImGui::MenuItem("Close", NULL, false, p_open != NULL)) *p_open = false;

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Edit")) {

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Preferences")) {
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


		void updateGUI(GUIParams *params) {
			
			// New frame must be created before any ImGui submission
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

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
			// Note: This has to be in between the Begin & End for the "Invisible Window"
			updateMenu();

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

#if 0
			// Another failed attempt to make the main node unmovable
			//ImGui::Begin("##DockNode_8CB0105", NULL, ImGuiWindowFlags_NoMove);
			//ImGui::End();
#endif


			// Process & update all GUIs
			NoxEngine::EngineGUI::updateAudioPanel(params);
			NoxEngine::EngineGUI::updateAnimationPanel(params);
			NoxEngine::EngineGUI::updateScenePanel(params);

			// ImGuizmo
			updateImGuizmoDemo(params);


			// Placeholder / debug windows
			ImGui::Begin(PANEL_NAME_MAP[PanelName::FileExplorer].c_str());    ImGui::End();
			ImGui::Begin(PANEL_NAME_MAP[PanelName::PresetObjects].c_str());   ImGui::End();
			ImGui::Begin(PANEL_NAME_MAP[PanelName::Hierarchy].c_str());		  ImGui::End();
			ImGui::ShowDemoWindow();
			ImGui::ShowMetricsWindow();

			// Finally show UI
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			// First loop is over
			params->firstLoop = false;
		}
	}

}