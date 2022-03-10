#define GLEW_STATIC
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <GL/glew.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include <Utils.h>
#include <EventManager.h>

using NoxEngineUtils::Logger;
using NoxEngine::EventManager;

int main(int argc, char** argv) {


	Logger::debug("hello world");

	EventManager *event_manager = EventManager::Instance();
	event_manager->addListener("meow", []() { Logger::debug("Hello World From event Meow"); });
	event_manager->addListener("meow", []() { Logger::debug("Second Hello world"); });
	event_manager->addListener("onDraw", []() { Logger::debug("func 2"); });
	event_manager->addListener("onDraw", []() { Logger::debug("func 1"); });
	event_manager->signal("onDraw");
	event_manager->signal("meow");

	return 0;
}
