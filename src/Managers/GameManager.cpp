#include <Managers/GameManager.h>
#include <Managers/LiveReloadManager.h>

#include <filesystem>
#include <Core/Entity.h>

// Components to hook up with event manager
#include <Components/TransformComponent.h>
#include <Components/RenderableComponent.h>
#include <Components/AnimationComponent.h>
#include <Components/AudioSourceComponent.h>
#include <Components/AudioListenerComponent.h>
#include <Components/AudioGeometryComponent.h>
#include <Components/ScriptComponent.h>

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
	keys(),
	game_state()
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
	init_scripts();
}

void GameManager::update() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	update_livereloads();
	update_inputs();
	update_ecs();
	update_renderer();
	update_gui();
	update_audio();
	update_animation();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window)) {
		should_close = true;
	}

	glfwSwapBuffers(window);
}

// Whenever an entity is created, modified, or deleted, call this function
// This is done to simplify code since previously we had renderer->addObject() everywhere
void GameManager::scheduleUpdateECS() {
	updateNeededECS = true;
}

// Create audio geometry from the entity's AudioGeometryComponent and add it to the renderer
void GameManager::createAudioGeometry(Entity* ent, IAudioGeometry* igeo) {

	// TODO: First remove this object from audio engine & renderer first
	// ...
	// audioManager->removeGeometry(igeo->geometryId);
	// renderer->removeObject(igeo->rendObjId);

	// Create one Geometry object with all meshes in the meshScene
	igeo->geometryId = audioManager->createGeometry(ent);

	// Add to renderer
	renderer->addObject(ent, igeo, ComponentType::AudioGeometryType);

	// Get the MeshScene reference (if it has one - creating plane / bounding box will not generate one), 
	// set the rendObjId of meshes to be the newly added object in the renderer
	if (igeo->meshScene) {
		for (u32 i = 0; i < igeo->meshScene->meshes.size(); i++) {
			igeo->meshScene->meshes[i]->rendObjId = igeo->rendObjId;
		}
	}

	// Update renderer
	// TODO-OPTIMIZATION: Set a flag inside GameManager, update buffers in a batch fashion
	this->renderer->updateBuffers();
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

			// Add to hash map if it does not exist
			if (game_state.meshScenes.find(file_name) == game_state.meshScenes.end()) {
				game_state.meshScenes.emplace(file_name, NoxEngine::readFBX(file_name.c_str()));
			}
			MeshScene& meshScene = game_state.meshScenes.find(file_name)->second;

			i32 index = game_state.activeScene->entities.size();
			// We're treating every mesh as an entity FOR NOW
			for (u32 i = 0; i < meshScene.meshes.size(); i++)
			{
				// Note (Vincent): this is more or less the same as letting the scene automatically allocate an entity,
				//                 because the entity ID is managed by the scene
				Entity* ent = new Entity(game_state.activeScene, std::filesystem::path(file_name).filename().string().c_str());

				RenderableComponent* comp = new RenderableComponent(*meshScene.meshes[i]);
				TransformComponent* trans = new TransformComponent(0.0, 0.0, 0.0);

				// ScriptComponent *script = new ScriptComponent("assets/scripts/test.lua");

				ent->addComp(comp);
				ent->addComp(trans);
				// ent->addComp(script);

				if (meshScene.hasAnimations())
				{
					// Get the node associated to this mesh
					for (MeshNode* node : meshScene.allNodes)
					{
						if (node->hasAnimations() && node->meshIndex[0] == i)
						{
							AnimationComponent* anim = new AnimationComponent(meshScene, node);
							ent->addComp(anim);
							break;
						}
					}
				}

				game_state.activeScene->addEntity(ent);
			}
	});


	EventManager::Instance()->addListener(EventNames::componentAdded, [this](va_list args) {

			Entity* ent = va_arg(args, Entity*);
			const std::type_index compTypeId = va_arg(args, std::type_index);

			// Renderer
			if (ent->containsComps<RenderableComponent>()) {

				RenderableComponent* rendComp = ent->getComp<RenderableComponent>();

				if (!renderer->hasRendObj(rendComp->rendObjId)) {

					renderer->addObject(ent, rendComp, ComponentType::RenderableType);

					this->renderer->updateBuffers();
				}
			}


			// Audio
			// Geometry: We don't add the mesh to the audio engine when the geometry component is added, 
			// because it's expected to be empty. We add to audio engine when mesh is loaded
			// AudioSource: No need to do anything special
			// AudioListener: first listener is set to active
			if (game_state.activeAudioListener == nullptr && compTypeId == typeid(AudioListenerComponent)) {
				game_state.activeAudioListener = ent;
			}

			// ...
	});


	EventManager::Instance()->addListener(EventNames::componentRemoved, [this](va_list args) {

			Entity* ent = va_arg(args, Entity*);
			const std::type_index compTypeId = va_arg(args, std::type_index);

			if (compTypeId == typeid(RenderableComponent)) {
				renderer->removeObject(ent->getComp<RenderableComponent>()->rendObjId);
				// TODO-OPTIMIZATION: Remove in batches every X ms, shift the still-valid indices to take the free space
			}

			// Audio
			if (compTypeId == typeid(AudioGeometryComponent)) {

				AudioGeometryComponent* geoComp = ent->getComp<AudioGeometryComponent>();
				IAudioGeometry* igeo = geoComp->CastType<IAudioGeometry>();

				// Disable the loaded geometry
				// TODO: remove it
				audioManager->setGeometryActive(igeo->geometryId, false);
				renderer->removeObject(ent->getComp<AudioGeometryComponent>()->rendObjId);
			}

	});
}

void GameManager::init_audio() {
	audioManager = AudioManager::Instance();
	audioManager->init();

	audioManager->set3dListenerAttributes(
			{ 0.0f, 0.0f, 0.0f },		// Position
			{ 0.0f, 0.0f, 0.0f },		// velocity (TODO: calculate)
			{ 0.0f, 0.0f, -1.0f },		// Forward
			{ 0.0f, 1.0f, 0.0f }		// Up
			);

	game_state.activeAudioListener = nullptr;
}

void GameManager::init_camera() {
	camera = new Camera(vec3(100.0f, 100.0f, 100.0f));
}

void GameManager::init_shaders() {

	programs.emplace_back(Array<ShaderFile>{
		{ "assets/shaders/vShader.glsl", GL_VERTEX_SHADER, 0 },
		{ "assets/shaders/fShader.glsl", GL_FRAGMENT_SHADER, 0 },
	});

	programs.push_back(Array<ShaderFile>{
		{"assets/shaders/vertexShader.vs", GL_VERTEX_SHADER, 0},
		{ "assets/shaders/fragmentShader.fs", GL_FRAGMENT_SHADER, 0 }
	});

	current_program = &programs[0];
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

	GridObject *obj = new GridObject(vec3(-500, 0, -500), vec3(500, 0, 500), 50);
	renderer->addPermObject(obj);
	renderer->updateBuffers();
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

void GameManager::init_scene() {
	game_state.scenes.push_back(new Scene());
	game_state.activeScene = game_state.scenes[0];
}

void NoxEngine::GameManager::init_scripts()
{


	// Steven: That's how I would do it
	// clean up: leaky mem
	String file_name = "assets/meshes/card.fbx";

	// Add to hash map if it does not exist
	if (game_state.meshScenes.find(file_name) == game_state.meshScenes.end()) {
		game_state.meshScenes.emplace(file_name, NoxEngine::readFBX(file_name.c_str()));
	}
	MeshScene& meshScene = game_state.meshScenes.find(file_name)->second;

	i32 index = game_state.activeScene->entities.size();
	// We're treating every mesh as an entity FOR NOW
	for (u32 i = 0; i < meshScene.meshes.size(); i++)
	{
		// Note (Vincent): this is more or less the same as letting the scene automatically allocate an entity,
		//                 because the entity ID is managed by the scene
		Entity* ent = new Entity(game_state.activeScene, std::filesystem::path(file_name).filename().string().c_str());

		RenderableComponent* comp = new RenderableComponent(*meshScene.meshes[i]);
		TransformComponent* trans = new TransformComponent(0.0, 0.0, 0.0);

		ScriptComponent *script = new ScriptComponent("assets/scripts/test.lua");

		ent->addComp(comp);
		ent->addComp(trans);
		ent->addComp(script);

		if (meshScene.hasAnimations())
		{
			// Get the node associated to this mesh
			for (MeshNode* node : meshScene.allNodes)
			{
				if (node->hasAnimations() && node->meshIndex[0] == i)
				{
					AnimationComponent* anim = new AnimationComponent(meshScene, node);
					ent->addComp(anim);
					break;
				}
			}
		}

		game_state.activeScene->addEntity(ent);
	}


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
	ImGui::Image((ImTextureID)(u64)renderer->getTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}

void GameManager::update_livereloads() {

	LiveReloadManager *lrManager = LiveReloadManager::Instance();
	lrManager->checkFiles();

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

void GameManager::update_ecs() {

	size_t nEntities = game_state.activeScene->entities.size();

	// script update
	const auto entities = game_state.activeScene->entities;
	for (i32 i = 0; i < nEntities; i++)
	{
		entities[i]->tick(deltaTime, currentTime);
	}


	if (!updateNeededECS) return;

	//bool updateRenderer = false;
	//bool updateAudioManager = false;

	bool entityRemoved = false;



	// Check for entity removal. 
	// Free resources explicitly (since `entities` contain raw `Entity*` pointers) and resize vector
	game_state.activeScene->entities.erase(
		std::remove_if(
			game_state.activeScene->entities.begin(),
			game_state.activeScene->entities.end(),
			[](auto&& ent) {
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

void GameManager::update_gui() {

	ImGuiIO& io = ImGui::GetIO();
	
	char windowTitle[64];
	snprintf(windowTitle, 64, "%s - FPS %.3f", title.c_str(), io.Framerate);
	glfwSetWindowTitle(window, windowTitle);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::PushFont(font);

	NoxEngineGUI::updateGUI(game_state, &ui_params);
	//NoxEngineGUI::updateAudioPanel(&game_state, &ui_params);
	NoxEngineGUI::updateAnimationPanel(&game_state, &ui_params);
	NoxEngineGUI::updatePresetObjectPanel(&game_state);
	NoxEngineGUI::updateScenePanel(&game_state);
	NoxEngineGUI::updateHierarchyPanel(&game_state, &ui_params);
	NoxEngineGUI::updateInspectorPanel(&game_state, &ui_params);
	NoxEngineGUI::updateSkyboxPanel(&game_state);

	// NoxEngineGUI::updateImGuizmoDemo(&ui_params);

	ImGui::PopFont();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void GameManager::update_audio() {

	// Set listener attributes
	vec3 pos{ 0 };
	vec3 vel{ 0 };
	vec3 forward(0.0f, 0.0f, -1.0f);
	vec3 up(0.0f, 1.0f, 0.0f);

	ITransform* itrans = nullptr;
	IAudioListener* ilisten = nullptr;
	if (game_state.activeAudioListener) {
		itrans	= game_state.activeAudioListener->getComp<TransformComponent>();
		ilisten	= game_state.activeAudioListener->getComp<AudioListenerComponent>();

		if (itrans) {
			mat4 rotation = glm::eulerAngleXYZ(itrans->rx, itrans->ry, itrans->rz);

			pos		= vec3(itrans->x, itrans->y, itrans->z);
			forward = rotation * vec4(forward, 1.0f);
			up		= rotation * vec4(up, 1.0f);
		}

		if (ilisten) vel = ilisten->vVel;
	} 

	//audioManager->set3dListenerAttributes(itrans);
	audioManager->set3dListenerAttributes(pos, vel, forward, up);

	// Update geometry states & orientation
	for (Entity* ent : game_state.activeScene->entities) {

		if (!ent->isEntityEnabled()) continue;

		// TODO: update ITransform to ITransform, then we can get rotation and scale as well
		ITransform*		itrans	= ent->getComp<TransformComponent>();
		IAudioSource*	isrc	= ent->getComp<AudioSourceComponent>();
		IAudioGeometry* igeo	= ent->getComp<AudioGeometryComponent>();

		// TODO: fix handedness
		vec3 pos{ 0 };
		vec3 rot{ 0 };
		vec3 scale{ 1 };
		//vec3 rot = glm::vec3(transform->rotx, transform->roty, transform->rotz);
		//vec3 scale = glm::vec3(transform->scalex, transform->scaley, transform->scalez);

		if (itrans && ent->isEnabled<TransformComponent>()) {
			pos = glm::vec3(itrans->x, itrans->y, itrans->z);
		}

		if (isrc) {

			// TODO: stop audio if it's disabled
			//if (!ent->isEnabled<AudioSourceComponent>()) audioManager->stopSound(0);

			// TODO: check audio is playing
			// TODO: set correct channel id (not 0)
			audioManager->setChannel3dPosition(0, pos);
			audioManager->setChannelVolume(0, isrc->volume);
		}

		if (igeo) {

			// Geometry component does not have a valid mesh (e.g. not yet loaded), don't do anything
			if (igeo->geometryId == -1) continue;

			// Set active
			audioManager->setGeometryActive(igeo->geometryId, ent->isEnabled<AudioGeometryComponent>());

			// Orient geometry
			if (ent->containsComps<TransformComponent>() && ent->isEnabled<TransformComponent>()) {

				audioManager->orientGeometry(igeo->geometryId, pos, rot, scale);
			}
		}
	}

	audioManager->update();
}


void GameManager::update_animation() {

	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	for (Entity* ent : game_state.activeScene->entities) { 

		AnimationComponent *animComp = ent->getComp<AnimationComponent>();

		if (animComp) {
			animComp->update(deltaTime);
		}
	}
}

void GameManager::update_renderer() {

	renderer->setFrameBufferToTexture();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	for (Entity* ent : game_state.activeScene->entities) {

		if (ent->containsComps<RenderableComponent, AnimationComponent>())
		{
			RenderableComponent *rendComp = ent->getComp<RenderableComponent>();
			AnimationComponent* animComp = ent->getComp<AnimationComponent>();

			mat4 transformation = animComp->getTransformation();
			renderer->updateObjectTransformation(transformation, rendComp->rendObjId);
		}
	}

	renderer->updateCamera();
	renderer->updateLightPos(game_state.light[0], game_state.light[1], game_state.light[2]);
	renderer->fillBackground(ui_params.sceneBackgroundColor);

	renderer->setProgram(&programs[1]);
	renderer->drawSkyBox();

	renderer->setProgram(current_program);
	renderer->draw();

}

////////////////////////////
/*   Different managers   */
////////////////////////////

bool GameManager::loadAudioSource(Entity* ent, IAudioSource *isrc) {

	String file_name = IOManager::Instance()->PickFile("All Files\0*.*\0\0");
	if (file_name.length() <= 0) return false;

	isrc->filePath = file_name;

	audioManager->loadSound(isrc);
	isrc->loaded = true;

	return true;
}

bool GameManager::loadAudioGeometry(Entity* ent, IAudioGeometry *igeo) {

	String file_name = IOManager::Instance()->PickFile("All Files\0*.*\0\0");
	if (file_name.length() <= 0) return false;

	game_state.meshScenes.emplace(file_name, NoxEngine::readFBX(file_name.c_str()));
	MeshScene& meshScene = game_state.meshScenes.find(file_name)->second;

	// Convert meshScene into vertices, face indices for Geometry triangles, and indices for renderer lines
	return igeo->loadMesh(&meshScene);
}

bool GameManager::playSound(Entity* ent, IAudioSource* isrc) {

	ITransform* itrans = ent->getComp<TransformComponent>();
	vec3 pos{ itrans->x, itrans->y, itrans->z };

	audioManager->loadSound(isrc);
	audioManager->playSounds(isrc->filePath, pos, isrc->volume);

	return true;
}

bool GameManager::stopSound(Entity* ent, IAudioSource* isrc) {

	//audioManager->stopSound(isrc);
	throw std::exception("impl");

	return true;
}
