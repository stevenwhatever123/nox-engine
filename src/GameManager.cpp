#include <GameManager.h>
#include <glm/gtx/string_cast.hpp>

using NoxEngineUtils::Logger;
using NoxEngine::EventManager;
using NoxEngine::Entity;

using namespace NoxEngine;
using namespace NoxEngineGUI;

GameManager::GameManager(u32 width, u32 height, String title) :
	win_width(width),
	win_height(height),
	title(title),
	scene(),
	should_close(false),
	keys()
{
}

void GameManager::init() {
	LOG_DEBUG("Initing systems");
	init_window();
	init_events();
	init_audio();
	init_camera();
	init_shaders();
	init_imgui();
	init_animation();

	glPointSize(4.0);

	init_renderer();
}

void GameManager::update() {
	glClear(GL_COLOR_BUFFER_BIT);

	update_inputs();
	update_audio();
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

void callback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam) {

	LOG_DEBUG("Message: %s", message);
}

void GameManager::init_window() {
	LOG_DEBUG("Initializing Window");

	if (!glfwInit()) {
		LOG_DEBUG("Error initializing glfw...exiting.");
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true );
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(win_width, win_height, title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(window);


	glfwSetWindowPos(window, 100, 100);

	if (window == nullptr) {
		LOG_DEBUG("Failed to create window");
		exit(1);
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_DEBUG("Failed to initialize OpenGL context");
	}


	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
	glDebugMessageCallback(callback, NULL);

	glfwSetWindowUserPointer(window, this);

	auto func = [](GLFWwindow *w, i32 key, i32 scan, i32 action, i32 mods){
		glfwGetWindowUserPointer(w);

		GameManager *gm = (GameManager *)glfwGetWindowUserPointer(w);
		if(action == GLFW_PRESS)
			gm->keys[(char)key] = 1;
		if(action == GLFW_RELEASE)
			gm->keys[(char)key] = 0;
	};

	glfwSetKeyCallback(window, func);

}


void GameManager::init_events() {

	EventManager::Instance()->addListener(EventNames::meshAdded, [this](va_list args){

			// Steven: That's how I would do it
			// clean up: leaky mem
			String file_name = va_arg(args, char*);
			this->game_state.meshScenes.emplace(file_name, NoxEngine::readFBX(file_name.c_str()));
			MeshScene &meshScene = this->game_state.meshScenes.find(file_name)->second;

			i32 index = this->scene.entities.size();
			// We're treating every mesh as an entity FOR NOW
			for (u32 i = 0; i < meshScene.meshes.size(); i++)
			{
				Entity *ent = new Entity();
				RenderableComponent* comp = meshScene.meshes[i];
				PositionComponent* pos = new PositionComponent(0.0, 0.0, 0.0);
				ent->addComp(comp);
				ent->addComp(pos);

				this->scene.addEntity(ent);

				this->renderer->addObject(
						reinterpret_cast<IRenderable*>(ent->getComp(2)->CastType(2)),
						reinterpret_cast<IPosition*>(ent->getComp(1)->CastType(2))
						);
			}

			this->renderer->updateBuffers();

	});


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
	camera = new Camera(vec3(0.0f, 10.0f, 150.0f));
	camera->turnVerBy(35.0f);
}

void GameManager::init_shaders() {

	programs.emplace_back(Array<ShaderFile>{
		{ "assets/shaders/vShader.glsl", GL_VERTEX_SHADER, 0 },
		{ "assets/shaders/fShader.glsl", GL_FRAGMENT_SHADER, 0 },
	});

	current_program = &programs.back();
}

void GameManager::init_animation() {
	currentTime = glfwGetTime();
	deltaTime = 0;
	lastTime = currentTime;
}

void GameManager::init_renderer() { 
	renderer = new Renderer(win_width, win_height, camera);
	renderer->setProgram(current_program);
	renderer->useProgram();
	game_state.renderer = renderer;
	renderer->setFrameBufferToTexture();
	// GridObject obj(vec3(-500, 0, -500), vec3(1000, 0, 1000), 500);

	// renderer->addObject(
	// 		static_cast<IRenderable*>(&obj),
	// 		static_cast<IPosition*>(&obj)
	// 		);


	// renderer->updateBuffers();
}

void GameManager::init_imgui() {

	NoxEngineGUI::init_imgui(window);
	
	ImGuiIO& io = ImGui::GetIO();
	font = io.Fonts->AddFontFromFileTTF("assets/font/envy.ttf", 18);
	io.Fonts->Build();

	// Initialize panel variables
	NoxEngineGUI::initPresetObjectPanel();

	ui_params.current_cam = camera;
	ui_params.sceneBackgroundColor = 0x282828FF;

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
	ImGui::Image((ImTextureID)(u64)renderer->getTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}

void GameManager::update_gui() {

	ImGuiIO& io = ImGui::GetIO();
	
	// Show FPS
	char windowTitle[64];
	snprintf(windowTitle, 64, "%s - FPS %.3f", title.c_str(), io.Framerate);
	glfwSetWindowTitle(window, windowTitle);

	// Draw window content
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::PushFont(font);
	
	NoxEngineGUI::updateGUI(&ui_params);
	NoxEngineGUI::updateAudioPanel(&game_state);
	NoxEngineGUI::updateAnimationPanel(&game_state);
	NoxEngineGUI::updatePresetObjectPanel(&game_state);
	NoxEngineGUI::updateScenePanel(&game_state);

	ImGui::PopFont();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void GameManager::update_audio() {

	// Sync audio manager with the game state's audio repo
	// TODO: Add ChannelID to AudioSource, iterate through all of them and 
	//       set the pos/volume in the appropriate ChannelGroup / ChannelControl
	for (auto itr : game_state.audioSources) {
		audioManager->SetChannel3dPosition(0, itr.second.position);
		audioManager->SetChannelVolume(0, itr.second.sourceVolume);
	}

	audioManager->Update();
}

void GameManager::update_inputs() {
	glfwPollEvents();


	if(keys['W']) { camera->moveFwdBy(0.1f); }
	if(keys['S']) { camera->moveFwdBy(-0.1f); }
	if(keys['D']) { camera->moveHorBy(-0.1f); }
	if(keys['A']) { camera->moveHorBy(0.1f); }
	if(keys[' ']) { camera->moveVerBy(0.1f); }
	if(keys['K']) { camera->moveVerBy(-0.1f); }

}

void GameManager::update_animation() {

	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	//auto meshStart = game_state.meshes.begin();
	//auto meshEnd = game_state.meshes.end();
	auto meshSceneStart = game_state.meshScenes.begin();
	auto meshSceneEnd = game_state.meshScenes.end();

	for(; meshSceneStart != meshSceneEnd; meshSceneStart++) 
	{
		if (meshSceneStart->second.playAnimation)
		{
			if (meshSceneStart->second.hasAnimations())
			{
				if (meshSceneStart->second.frameIndex
					== meshSceneStart->second.numTicks[meshSceneStart->second.animationIndex] - 1)
				{
					meshSceneStart->second.resetAnimation();
					meshSceneStart->second.playAnimation = !meshSceneStart->second.playAnimation;
				}
			}
			meshSceneStart->second.update(deltaTime);
		}
		// Because we're not updating frame index we still need tick floor and ceil to get our transform
		meshSceneStart->second.updateCeilAndFloor();
	}
}

void GameManager::update_renderer() {
	auto meshSceneStart = game_state.meshScenes.begin();
	auto meshSceneEnd = game_state.meshScenes.end();
	for (; meshSceneStart != meshSceneEnd; meshSceneStart++) 
	{
		MeshScene &currentMeshScene = meshSceneStart->second;
		for (u32 i = 0; i < meshSceneStart->second.allNodes.size(); i++)
		{
			MeshNode2* node = meshSceneStart->second.allNodes[i];

			if (node->meshIndex.size() > 0)
			{
				Mesh2* mesh = currentMeshScene.meshes[node->meshIndex[0]];
				if (node->hasAnimations())
				{
					glm::mat4 transformation = node->getGlobalTransformation(
						currentMeshScene.frameIndex, currentMeshScene.animationIndex,
						currentMeshScene.accumulator, currentMeshScene.timeStep,
						currentMeshScene.whichTickFloor, currentMeshScene.whichTickCeil);
					renderer->updateObjectTransformation(transformation, mesh);
				}
				else
				{
					glm::mat4 transformation = node->transformation;
					renderer->updateObjectTransformation(transformation, mesh);
				}
			}
		}
	}



	renderer->updateCamera();
	renderer->updateLightPos(game_state.light[0], game_state.light[1], game_state.light[2]);
	renderer->fillBackground(ui_params.sceneBackgroundColor);
	renderer->draw();

}

