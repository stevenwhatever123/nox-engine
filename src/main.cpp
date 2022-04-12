#define GLEW_STATIC
#include <Windows.h>
#include <iostream>
#include <future>
#include <vector>

#include <GL/glew.h>
#include <imgui/imgui_internal.h>	// for docking
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include "../FBXFileLoader.h"
#include "../ImGuiCustomWindow.h"
#include <fmod/core/fmod.hpp>
#include <fmod/core/fmod_errors.h>

#include "AudioManager.h"


//#include "Shader.h"
#include "Renderer.h"
#include "RenderableComp.h"


unsigned int winWidth = 1300 , winHeight = 1000;

GLFWwindow* initialize_window() {
	if (!glfwInit()) {
		std::cout << "Error initializing glfw...exiting.";
		exit(1);
	}
	

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* win = glfwCreateWindow(winWidth, winHeight, "Nox Engine", nullptr, nullptr);

	glfwMakeContextCurrent(win);
	
	if (win == nullptr) {
		std::cout << "Failed to create window" << std::endl;
		exit(1);
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << " Error Initializing Glew" << std::endl;
		exit(1);
	}

	return win;
}


AudioManager* init_audio(std::string audioFilePath) {
	// Initialize audio system
	// TODO: Change to singleton
	AudioManager* audioManager = new AudioManager();
	audioManager->Init();

	// Set listener. TODO: Move this inside the engine loop
	audioManager->Set3dListenerAttributes(
			{ 0.0f, 0.0f, 0.0f },		// Position
			{ 0.0f, 0.0f, 0.0f },		// velocity (TODO: calculate)
			{ 0.0f, 0.0f, 1.0f },		// Forward
			{ 0.0f, 1.0f, 0.0f }		// Up
			);

	// Load an audio file
	
	audioManager->LoadSound(audioFilePath, true);

	return audioManager;
}

// This is expected to run ONCE, AFTER an ImGui::NewFrame is created
// Example by AidanSun05: https://gist.github.com/AidanSun05/953f1048ffe5699800d2c92b88c36d9f
void setup_imgui_docking() {    
	// 2. We want our whole dock node to be positioned in the center of the window
	ImVec2 workCenter = ImGui::GetMainViewport()->GetWorkCenter();

	// 3. Now we'll need to create our dock node:
	ImGuiID id = ImGui::GetID("MainWindowGroup"); // The string chosen here is arbitrary (it just gives us something to work with)
	ImGui::DockBuilderRemoveNode(id);             // Clear any preexisting layouts associated with the ID we just chose
	ImGui::DockBuilderAddNode(id);                // Create a new dock node to use

	// 4. Set the dock node's size and position:
	ImVec2 size{ 600, 300 }; // A decently large dock node size (600x300px) so everything displays clearly

	// Calculate the position of the dock node
	//
	// `DockBuilderSetNodePos()` will position the top-left corner of the node to the coordinate given.
	// This means that the node itself won't actually be in the center of the window; its top-left corner will.
	//
	// To fix this, we'll need to subtract half the node size from both the X and Y dimensions to move it left and up.
	// This new coordinate will be the position of the node's top-left corner that will center the node in the window.
	ImVec2 nodePos{ workCenter.x - size.x * 0.5f, workCenter.y - size.y * 0.5f };

	// Set the size and position:
	ImGui::DockBuilderSetNodeSize(id, size);
	ImGui::DockBuilderSetNodePos(id, nodePos);

	// 5. Split the dock node to create spaces to put our windows in:

	// Split the dock node in the left direction to create our first docking space. This will be on the left side of the node.
	// (The 0.5f means that the new space will take up 50% of its parent - the dock node.)
	ImGuiID dock1 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.5f, nullptr, &id);
	// +-----------+
	// |           |
	// |     1     |
	// |           |
	// +-----------+

	// Split the same dock node in the right direction to create our second docking space.
	// At this point, the dock node has two spaces, one on the left and one on the right.
	ImGuiID dock2 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.5f, nullptr, &id);
	// +-----+-----+
	// |     |     |
	// |  1  |  2  |
	// |     |     |
	// +-----+-----+
	//    split ->

	// For our last docking space, we want it to be under the second one but not under the first.
	// Split the second space in the down direction so that we now have an additional space under it.
	//
	// Notice how "dock2" is now passed rather than "id".
	// The new space takes up 50% of the second space rather than 50% of the original dock node.
	ImGuiID dock3 = ImGui::DockBuilderSplitNode(dock2, ImGuiDir_Down, 0.5f, nullptr, &dock2);
	// +-----+-----+
	// |     |  2  |  split
	// |  1  +-----+    |
	// |     |  3  |    V
	// +-----+-----+

	// 6. Add windows to each docking space:
	ImGui::DockBuilderDockWindow("One", dock1);
	ImGui::DockBuilderDockWindow("Two", dock2);
	ImGui::DockBuilderDockWindow("Three", dock3);

	// 7. We're done setting up our docking configuration:
	ImGui::DockBuilderFinish(id);
}


void init_imgui(GLFWwindow* win) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	// Config flags
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

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

	//setup_imgui_docking();
}


Renderer* init_renderer(Camera* camera) {
	// ------------------------------ Set up of the render --------------------------
	
	
	

	// Create Renderer
	Renderer* renderer = new Renderer(winWidth, winHeight, camera);



	// Add objects to Renderer
	//renderer->addObject(obj);
	//renderer->addObject(obj2);
	//renderer->addObject(obj3);

	// Prep renderers buffers after setting uo objs
	//renderer->updateBuffers();
	renderer->setFrameBufferToTexture();

	return renderer;
}


int main(int argc, char** argv) {

	bool firstLoop = true;

	// Initialize GLFW
	GLFWwindow* win = initialize_window();
	bool should_close = false;
	glClearColor(0.2f, 0.6f, 0.5f, 1.0f);

	bool open_demo = true;
	glewInit();
	
	std::string audioFilePath = "assets/bgm.ogg";
	AudioManager* audioManager = init_audio(audioFilePath);

	// new event system in imgui
	init_imgui(win);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	


	//ImGuiCustomWindow* customWindow = new ImGuiCustomWindow();

	Mesh* mesh = NULL;
	bool isAlreadyLoaded = false;

	// Used to detect change in window size
	int locWidth = winWidth, locHeight = winHeight, prevWidth = winWidth, prevHeight = winHeight;

	// Create Renderable Objects
	RenderableComp* obj = new RenderableComp(0.0f, 0.0f, 0.0f, "textures/Solid_white.png");
	RenderableComp* obj2 = new RenderableComp(3.0f, 0.0f, 2.0f, "textures/Red.png");
	RenderableComp* obj3 = new RenderableComp(-3.0f, 0.0f, -2.0f, "textures/beije.png");
	
	// Create Camera
	Camera* camera = new Camera(glm::vec3(0.0f, 10.0f, 200.0f));

	Renderer* renderer = init_renderer(camera);

	while (!should_close) {
		
		glfwPollEvents();

		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(win)) {
			should_close = true;
		}

		if (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS) {
			audioManager->PlaySounds(audioFilePath);
		}

		// Audio
		//future<void> fut = async(audioUpdate, audioManager);   // doesn't work?
		audioManager->Update();		// choppy audio sometimes for some reason?

		// GUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (firstLoop) {
			setup_imgui_docking();

			firstLoop = false;
		}

		/* {
			bool* p_open = nullptr;

			static bool opt_fullscreen = true;
			static bool opt_padding = false;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}
			else
			{
				dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
			}

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
			// and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			if (!opt_padding)
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				ImGui::Begin("DockSpace Demo", p_open, window_flags);
			if (!opt_padding)
				ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// Submit the DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Options"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.
					ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
					ImGui::MenuItem("Padding", NULL, &opt_padding);
					ImGui::Separator();

					if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
					if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
					if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
					if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
					if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
					ImGui::Separator();

					if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
						*p_open = false;
					ImGui::EndMenu();
				}
				
				ImGui::EndMenuBar();
			}

			ImGui::End();
		}*/

		//// Sliders to control the 3D sound position
		//static float soundX = 0.0f, soundY = 0.0f, soundZ = 0.0f;
		//static float soundVolume = 0.0f;
		//ImGui::SliderFloat("Sound X", &soundX, -1.0f, 1.0f, "X = %.3f");
		//ImGui::SliderFloat("Sound Y", &soundY, -1.0f, 1.0f, "Y = %.3f");
		//ImGui::SliderFloat("Sound Z", &soundZ, -1.0f, 1.0f, "Z = %.3f");
		//ImGui::SliderFloat("Sound Volume", &soundVolume, -20.0f, 2.0f, "Volume (dB) = %.3f");

		//// Update systems with UI values
		//audioManager->SetChannel3dPosition(0, {soundX, soundY, soundZ});
		//audioManager->SetChannelVolume(0, soundVolume);
		////customWindow->showFBXLoaderMenu(mesh);


		//ImGui::Begin("Settings");

		//if (ImGui::Button("Load FBX File"))
		//{
		//	OPENFILENAME ofn;

		//	wchar_t szFile[256];

		//	ZeroMemory(&ofn, sizeof(ofn));
		//	ofn.lStructSize = sizeof(ofn);
		//	ofn.hwndOwner = NULL;
		//	ofn.lpstrFile = szFile;
		//	ofn.lpstrFile[0] = '\0';
		//	ofn.nMaxFile = sizeof(szFile);
		//	ofn.lpstrFilter = L"Filmbox File (*.fbx)\0*.fbx\0All (*.*)\0*.*\0";
		//	ofn.nFilterIndex = 1;
		//	ofn.lpstrFileTitle = NULL;
		//	ofn.nMaxFileTitle = 0;
		//	ofn.lpstrInitialDir = NULL;
		//	ofn.lpstrTitle = L"Select FBX file";
		//	ofn.lpstrDefExt = L"fbx";
		//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		//	bool readSuccess = GetOpenFileName(&ofn);

		//	// LPWSTR is different from char
		//	// Therefore we're converting it
		//	char filenameBuffer[256];
		//	wcstombs_s(nullptr, filenameBuffer, sizeof(filenameBuffer), szFile, sizeof(szFile));

		//	if (readSuccess)
		//	{
		//		printf("Read Success\n");

		//		const aiScene* pScene = FBXFileLoader::readFBX(filenameBuffer);

		//		// Converting the scene to mesh data
		//		if (pScene != nullptr)
		//		{
		//			mesh = FBXFileLoader::getMesh(pScene);
		//			isAlreadyLoaded = true;
		//			//curMesh = mesh;
		//		}
		//		else
		//		{
		//			printf("Error: converting sence to mesh data");
		//		}
		//	}
		//	else
		//	{
		//		printf("Error: reading fbx file");
		//	}
		//}

		//ImGui::End();


		// If there was a mesh loaded by user
		if (isAlreadyLoaded){
			// Add mesh to renderer
			mesh->prepForRenderer();
			renderer->addObject(mesh);
			renderer->updateBuffers();
			isAlreadyLoaded = false;

		}


		//	Render to texture	
		if (locWidth != prevWidth || locHeight != prevHeight)
		{
			renderer->updateProjection(locWidth, locHeight);
			prevWidth = locWidth;
			prevHeight = locHeight;
		}

		//Render background of the app
		renderer->fillBackground(1.0f, 0.5f, 0.9f);
		renderer->draw();

		// Use IMGUI to show rendered to framebuffer 
		//ImGui::Begin("Demo window"); {

		//	// Get the size of the child (i.e. the whole draw size of the windows).
		//	ImVec2 wsize = ImGui::GetWindowSize();
		//	locWidth = wsize.x;
		//	locHeight = wsize.y;


		//	// Pass texture rendered to to ImGUI
		//	ImGui::Image((ImTextureID)renderer->getTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

		//} ImGui::End();

		ImGui::Begin("One");
		ImGui::End();

		ImGui::Begin("Two");
		ImGui::End();

		ImGui::Begin("Three");
		ImGui::End();

		// Finally show UI
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			//GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			//glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(win);
	}


	// Clean up
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(win);
	glfwTerminate();

	audioManager->Destroy();
	delete obj, obj2, obj3, camera, renderer;

	return 0;
}
