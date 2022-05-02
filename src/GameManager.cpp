#include <GameManager.h>
#include <FullscreenShader.h>

#include <filesystem>
#include <Entity.h>

// Components to hook up with event manager
#include <RenderableComponent.h>
#include <PositionComponent.h>

using NoxEngineUtils::Logger;
using NoxEngine::EventManager;
using NoxEngine::Entity;

using namespace NoxEngine;
using namespace NoxEngineGUI;

GameManager::GameManager() :
	win_width(WINDOW_WIDTH),
	win_height(WINDOW_HEIGHT),
	title(WINDOW_TITLE),
	ui_params(),
	should_close(false),
	keys()
{
}

void GameManager::init() {
	LOG_DEBUG("Initing systems");
	init_window();
	init_scene();
	init_ecs();
	init_events();
	init_audio();
	init_camera();
	init_shaders();
	init_gui();
	init_animation();
	init_renderer();
	init_postprocess();
}

void GameManager::update() {
	update_inputs();
	update_ecs();
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

// Whenever an entity is created, modified, or deleted, call this function
// This is done to simplify code since previously we had renderer->addObject() everywhere
void GameManager::scheduleUpdateECS() {
	updateNeededECS = true;
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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
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


void GameManager::init_scene() {
	game_state.scenes.push_back(new Scene());
	game_state.activeScene = game_state.scenes[0];
}



// Similar to activateScene
void GameManager::init_ecs() {

	initComponentTypes();

	// cleanup all subsystems

	// loop through all existing entities (loaded from a file?)
		// add to subsystems

	// prepare entities for subsystems
	update_ecs();
}


void GameManager::init_events() {

	EventManager::Instance()->addListener(EventNames::meshAdded, [this](va_list args){

			// Steven: That's how I would do it
			// clean up: leaky mem
			String file_name = va_arg(args, char*);
			game_state.meshScenes.emplace(file_name, NoxEngine::readFBX(file_name.c_str()));
			MeshScene &meshScene = game_state.meshScenes.find(file_name)->second;

			i32 index = game_state.activeScene->entities.size();
			// We're treating every mesh as an entity FOR NOW
			for (u32 i = 0; i < meshScene.meshes.size(); i++)
			{
				// Note (Vincent): this is more or less the same as letting the scene automatically allocate an entity,
				//                 because the entity ID is managed by the scene
				Entity* ent = new Entity(game_state.activeScene, std::filesystem::path(file_name).filename().string().c_str());

				RenderableComponent* comp = meshScene.meshes[i];
				PositionComponent* pos = new PositionComponent(0.0, 0.0, 0.0);
				ent->addComp(comp);
				ent->addComp(pos);

				game_state.activeScene->addEntity(ent);
			}
	});


	EventManager::Instance()->addListener(EventNames::componentAdded, [this](va_list args) {

		Entity* ent = va_arg(args, Entity*);
		const std::type_index compTypeId = va_arg(args, std::type_index);

		// Renderer
		if (ent->containsComps<PositionComponent, RenderableComponent>()) {

			RenderableComponent* rendComp = ent->getComp<RenderableComponent>();
			IRenderable* rend = rendComp->CastType<IRenderable>();

			if (!rend->registered) {
				renderer->addObject(ent);
				rend->registered = true;

				// Update renderer
				// TODO-OPTIMIZATION: Set a flag inside GameManager, update buffers in a batch fashion
				this->renderer->updateBuffers();
			}
		}

		// Audio
		// ...
	});

	EventManager::Instance()->addListener(EventNames::componentRemoved, [this](va_list args) {

		Entity* ent = va_arg(args, Entity*);
		const std::type_index compTypeId = va_arg(args, std::type_index);

		if (compTypeId == typeid(RenderableComponent)) {
			renderer->removeObject(ent);
			// TODO-OPTIMIZATION: Remove in batches every X ms, shift the still-valid indices to take the free space
		}

		// Audio
		// ...
	});
}

void GameManager::init_audio() {
	audioManager = AudioManager::Instance();
	audioManager->Init();

	audioManager->Set3dListenerAttributes(
			{ 0.0f, 0.0f, 0.0f },		// Position
			{ 0.0f, 0.0f, 0.0f },		// velocity (TODO: calculate)
			{ 0.0f, 0.0f, 1.0f },		// Forward
			{ 0.0f, 1.0f, 0.0f }		// Up
			);

}

void GameManager::init_camera() {
	camera = new Camera(vec3(10.0f, 10.0f, 10.0f));
}

void GameManager::init_shaders() {

	programs.emplace_back(Array<ShaderFile>{
		{ "assets/shaders/vShader.glsl", GL_VERTEX_SHADER, 0 },
		{ "assets/shaders/fShader.glsl", GL_FRAGMENT_SHADER, 0 },
	});



}

void GameManager::init_gui() {

	NoxEngineGUI::init_imgui(window);
	
	ImGuiIO& io = ImGui::GetIO();
	font = io.Fonts->AddFontFromFileTTF("assets/font/envy.ttf", 18);
	io.Fonts->Build();

	// Initialize panel variables
	NoxEngineGUI::initPresetObjectPanel();

	// Initialize gui params
	ui_params.selectedEntity = -1;
	ui_params.current_cam = camera;
	ui_params.sceneBackgroundColor = 0x282828FF;
}

void GameManager::init_animation() {
	currentTime = glfwGetTime();
	deltaTime = 0;
	lastTime = currentTime;
}

void GameManager::init_renderer() { 
	renderer = new Renderer(win_width, win_height, camera);
	renderer->setProgram(&programs[0]);
	renderer->useProgram();
	game_state.renderer = renderer;
	renderer->setFrameBufferToTexture();
	game_state.texture_used = renderer->getTexture();

	GridObject obj(vec3(-500, 0, -500), vec3(1000, 0, 1000), 1000);

	i32 indices[6] = { 0, 1, 2, 0, 2, 3 };

	glGenVertexArrays(1, &random_vao);
	glBindVertexArray(random_vao);
	glGenBuffers(1, &quad_index_buffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i32)*6, indices, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameManager::init_postprocess() { 

	game_state.post_processors.push_back(
		FullscreenShader{
			"assets/shaders/fullScreenShader.frag",
			Array<TextureInput>{
				{ renderer->getTexture(), 0},
			},
			win_width,
			win_height
		}
	);


	game_state.post_processors.push_back(
		FullscreenShader{
			"assets/shaders/fullScreenShader2.frag",
			Array<TextureInput>{
				{ game_state.post_processors.back().GetTexture(), 0},
			},
			win_width,
			win_height
		}
	);


	game_state.current_post_processor = &game_state.post_processors.back();
}

void GameManager::update_gui() {

	ImGuiIO& io = ImGui::GetIO();
	
	char windowTitle[64];
	snprintf(windowTitle, 64, "%s - FPS %.3f", title.c_str(), io.Framerate);
	glfwSetWindowTitle(window, windowTitle);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::PushFont(font);
	
	NoxEngineGUI::updateGUI(&ui_params);
	NoxEngineGUI::updateAudioPanel(&game_state);
	NoxEngineGUI::updateAnimationPanel(&game_state);
	NoxEngineGUI::updatePresetObjectPanel(&game_state);
	NoxEngineGUI::updateScenePanel(&game_state);
	NoxEngineGUI::updateHierarchyPanel(&game_state, &ui_params);
	NoxEngineGUI::updateInspectorPanel(&game_state, &ui_params);
	// NoxEngineGUI::updateImGuizmoDemo(&ui_params);

	ImGui::PopFont();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GameManager::update_ecs() {

	if (!updateNeededECS) return;

	//bool updateRenderer = false;
	//bool updateAudioManager = false;

	bool entityRemoved = false;
	size_t nEntities = game_state.activeScene->entities.size();


	// Check for entity removal. 
	// Free resources explicitly (since `entities` contain raw `Entity*` pointers) and resize vector
	game_state.activeScene->entities.erase(
		std::remove_if(
			game_state.activeScene->entities.begin(),
			game_state.activeScene->entities.end(),
			[](auto &&ent) { 
				bool rm = ent->remove;
				if (rm) delete ent;
				return rm;
			}
		),
		game_state.activeScene->entities.end()
	);

	entityRemoved = nEntities != game_state.activeScene->entities.size();


	// update subsystems if needed
	if (/*updateRenderer && */entityRemoved) renderer->updateBuffers();
	//if (updateAudioManager) audioManager->...

	// Update done
	updateNeededECS = false;
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

	if(keys['0']) {
		use_pp = 0;
		game_state.texture_used = renderer->getTexture();
	}

	if(keys['1']) {
		use_pp = 1;
		game_state.texture_used = game_state.post_processors[0].GetTexture();
	}

	if(keys['2']) {
		use_pp = 1;
		game_state.texture_used = game_state.post_processors[1].GetTexture();
	}


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
	// auto meshSceneStart = game_state.meshScenes.begin();
	// auto meshSceneEnd = game_state.meshScenes.end();
	// for (; meshSceneStart != meshSceneEnd; meshSceneStart++) 
	// {
	// 	MeshScene &currentMeshScene = meshSceneStart->second;
	// 	for (u32 i = 0; i < meshSceneStart->second.allNodes.size(); i++)
	// 	{
	// 		MeshNode2* node = meshSceneStart->second.allNodes[i];

	// 		if (node->meshIndex.size() > 0)
	// 		{
	// 			Mesh2* mesh = currentMeshScene.meshes[node->meshIndex[0]];
	// 			if (node->hasAnimations())
	// 			{
	// 				glm::mat4 transformation = node->getGlobalTransformation(
	// 					currentMeshScene.frameIndex, currentMeshScene.animationIndex,
	// 					currentMeshScene.accumulator, currentMeshScene.timeStep,
	// 					currentMeshScene.whichTickFloor, currentMeshScene.whichTickCeil);
	// 				renderer->updateObjectTransformation(transformation, mesh);
	// 			}
	// 			else
	// 			{
	// 				glm::mat4 transformation = node->transformation;
	// 				renderer->updateObjectTransformation(transformation, mesh);
	// 			}
	// 		}
	// 	}
	// }



	renderer->useProgram();
	renderer->updateCamera();
	renderer->updateLightPos(game_state.light[0], game_state.light[1], game_state.light[2]);
	renderer->fillBackground(ui_params.sceneBackgroundColor);
	renderer->draw();


	// renderer->setProgram(programs[1]);
	// renderer->setProgram(programs[1]);
	if(use_pp == 1) {
		glBindVertexArray(random_vao);
		// glClearColor(0.5f, 0.2f, 0.0f, 1.0f);
		// glClear(GL_COLOR_BUFFER_BIT);
		for(auto post_process : game_state.post_processors) {
			post_process.draw(currentTime);
		}
	}
}

