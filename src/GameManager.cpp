#include <GameManager.h>

using NoxEngineUtils::Logger;
using NoxEngine::EventManager;

using namespace NoxEngine;
using namespace NoxEngineGUI;

GameManager::GameManager(u32 width, u32 height, String title) : win_width(width), win_height(height), title(title) {
}

void GameManager::init() {
	Logger::debug("Initing systems");
	init_window();
	init_audio();
	init_camera();
	init_shaders();
	init_imgui();
	init_animation();
	init_renderer();
}

void GameManager::update() {
	glClear(GL_COLOR_BUFFER_BIT);

	update_inputs();
	// update_audio();
	update_renderer();
	update_gui();

	update_animation();

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

void GameManager::addMesh(String name, Mesh m) {

	//m.prepForRenderer();
	renderer->addObject(&m);
	renderer->updateBuffers();

	// game_state.addMesh().emplace(name, )

	// game_state.audioSources.emplace(audioSource.name, audioSource);
	// audioManager->LoadSound(audioSource.file);
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
	camera = new Camera(glm::vec3(0.0f, 1.0f, 4.0f));

	// camera->turnVerBy(90.0f);
}

void GameManager::init_shaders() {
	programs.emplace_back(std::vector<ShaderFile>{
			{ "assets/shaders/vShader.glsl", GL_VERTEX_SHADER, 0 },
			{ "assets/shaders/fShader.glsl", GL_FRAGMENT_SHADER, 0 },
			});

	current_program = &programs.back();
}

void GameManager::init_animation() {

	EventManager::Instance()->addListener("mesh_added", [this](va_list args){

		String file_name = va_arg(args, std::string);
		Mesh* mesh = va_arg(args, Mesh*);

		this->renderer->addObject(mesh);
		this->renderer->updateBuffers();

		Logger::debug("Yes but also no %s", file_name.c_str());

	});

	currentTime = glfwGetTime();
	deltaTime = 0;
	lastTime = currentTime;
}

void GameManager::init_renderer() { 
	// ------------------------------ Set up of the render --------------------------
	// // Create Renderer
	renderer = new Renderer(win_width, win_height, camera);
	renderer->setProgram(current_program);
	renderer->useProgram();

	game_state.renderer = renderer;

	// const aiScene* pScene = NoxEngine::readFBX("assets/meshes/card.fbx");
	// Mesh *mesh = NoxEngine::getMesh(pScene);

	// mesh->prepForRenderer();

	// renderer->addObject(mesh);
	// renderer->updateBuffers();
	// delete mesh;
}

void GameManager::init_imgui() {

	NoxEngineGUI::init_imgui(window);
	
	ImGuiIO& io = ImGui::GetIO();
	font = io.Fonts->AddFontFromFileTTF("envy.ttf", 18);
	io.Fonts->Build();


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

	NoxEngineGUI::updateGUI(&ui_params);
	NoxEngineGUI::updateAudioPanel(&game_state);
	NoxEngineGUI::updateAnimationPanel(&game_state);
	NoxEngineGUI::updateScenePanel(&game_state);

	ImGui::Begin("Light Settings");

	ImGui::DragFloat3("Position", game_state.light);

	ImGui::End();

	// main_contex_ui();

	ImGui::PopFont();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void GameManager::update_audio() {
	audioManager->Update();
}

void GameManager::update_inputs() {
	glfwPollEvents();
}

void GameManager::update_animation() {

	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	auto meshStart = game_state.meshes.begin();
	auto meshEnd = game_state.meshes.end();

	for(;meshStart != meshEnd; meshStart++) {
		meshStart->second.update(deltaTime);
	}
}

void GameManager::update_renderer() {
	renderer->updateLightPos(game_state.light[0], game_state.light[1], game_state.light[2]);
	renderer->fillBackground(0.1f, 0.2f, 0.5f);
	renderer->draw();
}

