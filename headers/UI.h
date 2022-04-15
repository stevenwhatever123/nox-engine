#pragma once

#define GLEW_STATIC
#include <Windows.h>
#include <iostream>
#include <future>
#include <vector>

#include <GL/glew.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include "FBXFileLoader.h"
#include "ImGuiCustomWindow.h"
#include <fmod/core/fmod.hpp>
#include <fmod/core/fmod_errors.h>

#include "AudioManager.h"
#include"Entity.h"

#include"UIState.h"
#include "Manager.h"


//#include "Shader.h"
//#include "Renderer.h"
#include "RenderableComp.h"
#include "PositionComp.h"


class UI {

	unsigned int winWidth = 1900, winHeight = 1000;

	GLFWwindow* win;

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


	void init_imgui(GLFWwindow* w) 
	{
		win = w;
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOther(win, true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}


public:

	UIState* curState;

	UI(UIState *st, GLFWwindow* win)
	{
		//win = initialize_window();
		//glewInit();
		init_imgui(win);

		// Set UI state
		curState = st;



	}

	void getInput()
	{
		glfwPollEvents();

		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(win)) {
			curState->isRunning = false;
		}
	}

	void update()
	{

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//if (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS) {
		//	audioManager->PlaySounds(audioFilePath);
		//}

		// One more window just cause
		ImGui::Begin("Light Position");


		ImGui::SliderFloat("slider x", &curState->lightX, -100.0f, 100.0f, " %.3f");
		ImGui::SliderFloat("slider y", &curState->lightY, -100.0f, 100.0f, " %.3f");
		ImGui::SliderFloat("slider z", &curState->lightZ, -100.0f, 100.0f, " %.3f");

		ImGui::End();




		// Use IMGUI to show rendered to framebuffer 
		ImGui::Begin("Demo window"); {

			// Get the size of the child (i.e. the whole draw size of the windows).
			ImVec2 wsize = ImGui::GetWindowSize();


			if (curState->winWidth != wsize.x || curState->winHeight != wsize.y)
			{
				curState->isWinSizeChanged = true;
				curState->winWidth = wsize.x;
				curState->winHeight = wsize.y;
			}


			// Pass texture rendered to to ImGUI
			ImGui::Image((ImTextureID)curState->textureToRenderTo, wsize, ImVec2(0, 1), ImVec2(1, 0));

		} ImGui::End();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(win);
	}


};