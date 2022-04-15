#include <GameManager.h>
#include <Utils.h>
#include <FBXFileLoader.h>

using NoxEngineUtils::Logger;
using namespace NoxEngine;

GameManager::GameManager(u32 width, u32 height, std::string title) : win_width(width), win_height(height), title(title) {

}

void GameManager::init() {
	Logger::debug("Initing systems");
	init_window();
	init_audio();
	init_camera();
	init_shaders();
	init_imgui();
	init_renderer();
}

void GameManager::update() {
	glClear(GL_COLOR_BUFFER_BIT);

	update_inputs();
	// update_audio();
	update_renderer();
	update_gui();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window)) {
		should_close = true;
	}

	glfwSwapBuffers(window);

}

void GameManager::addAudioSource(AudioSource audioSource) {

	// TODO(sharo): handle duplicates better
	game_state.audioSources.emplace(audioSource.name, audioSource);
	audioManager->LoadSound(audioSource.file);

}

void GameManager::init_window() {

	Logger::debug("Initializing Window");

	if (!glfwInit()) {
		std::cout << "Error initializing glfw...exiting.";
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(win_width, win_height, title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(window);

	if (window == nullptr) {
		std::cout << "Failed to create window" << std::endl;
		exit(1);
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << " Error Initializing Glew" << std::endl;
		exit(1);
	}

}

void GameManager::init_audio() {
	// Initialize audio system

	audioManager = AudioManager::Instance();

	// TODO: Change to singleton
	audioManager->Init();

	// Set listener. TODO: Move this inside the engine loop
	audioManager->Set3dListenerAttributes(
			{ 0.0f, 0.0f, 0.0f },		// Position
			{ 0.0f, 0.0f, 0.0f },		// velocity (TODO: calculate)
			{ 0.0f, 0.0f, 1.0f },		// Forward
			{ 0.0f, 1.0f, 0.0f }		// Up
			);

}

void GameManager::init_camera() {
	camera = new Camera(glm::vec3(10.0f, 200.0f, 0.0f));
	camera->turnVerBy(90.0f);
}

void GameManager::init_shaders() {
	programs.emplace_back(std::vector<ShaderFile>{
			{ "assets/shaders/vShader.glsl", GL_VERTEX_SHADER, 0 },
			{ "assets/shaders/fShader.glsl", GL_FRAGMENT_SHADER, 0 },
			});

	current_program = &programs.back();
}

void GameManager::init_renderer() { 
	// ------------------------------ Set up of the render --------------------------
	// // Create Renderer
	renderer = new Renderer(win_width, win_height, camera);
	renderer->setProgram(current_program);
	renderer->useProgram();

	const aiScene* pScene = NoxEngine::readFBX("assets/meshes/card.fbx");
	Mesh *mesh = NoxEngine::getMesh(pScene);

	mesh->prepForRenderer();

	renderer->addObject(mesh);
	renderer->updateBuffers();
	delete mesh;
}

void GameManager::init_imgui() {


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	font = io.Fonts->AddFontFromFileTTF("envy.ttf", 18);
	io.Fonts->Build();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOther(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");

}

void GameManager::asset_ui() {

	ImGui::Begin("Audio Sources");

	auto startItr = game_state.audioSources.begin();
	auto endItr = game_state.audioSources.end();

	ImGui::DragFloat3("Light", light);

	static bool show_demo_window = true;
	if(ImGui::Button("load")) {
		std::string file_name = IOManager::Instance()->PickFile("All Files\0*.*\0\0");
		Logger::debug("name %s", file_name.c_str());
	}

	if(ImGui::TreeNode("Audios")) {

		while(startItr != endItr) {
			AudioSource *audio_source = &startItr->second;
			if(ImGui::TreeNode(audio_source->name.c_str())) {
				ImGui::Text("File Name: %s", audio_source->file.c_str());
				if(ImGui::TreeNode("Position")) {
					ImGui::DragFloat3("Position", &audio_source->position[0]);
					ImGui::TreePop();
				}

				ImGui::SliderFloat("Volume: ", &audio_source->sourceVolume, -20.0f, 2.0f, "Volume (dB) = %.3f");

				ImGui::TreePop();
			}

			startItr++;
		}

		ImGui::TreePop();
	}

	ImGui::End();

}

void GameManager::main_contex_ui() {

	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoTitleBar  |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoCollapse;

	ImGui::SetNextWindowPos(ImVec2( 100, 100 ), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2( 100, 100 ), ImGuiCond_FirstUseEver);

	ImGui::Begin("Player", NULL, flags);

	ImVec2 wsize = ImGui::GetWindowSize();
	f32 locWidth = wsize.x;
	f32 locHeight = wsize.y;


	// Pass texture rendered to to ImGUI
	ImGui::Image((ImTextureID)renderer->getTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}

void GameManager::update_gui() {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::PushFont(font);

	asset_ui();
	main_contex_ui();

	ImGui::PopFont();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void GameManager::update_audio() { }

void GameManager::update_inputs() {
	glfwPollEvents();
}

void GameManager::update_renderer() {
	renderer->updateLightPos(light[0], light[1], light[2]);
	renderer->fillBackground(0.1, 0.2, 0.5);
	renderer->draw();
}

