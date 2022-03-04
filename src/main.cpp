#define GLEW_STATIC
#include <Windows.h>
#include <iostream>

#include <GL/glew.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>



GLFWwindow* initialize_window() {
	if (!glfwInit()) {
		std::cout << "Error initializing glfw...exiting.";
		exit(1);
	}
	

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* win = glfwCreateWindow(500, 500, "Nox Engine", nullptr, nullptr);
	
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

int main(int argc, char** argv) {
	GLFWwindow* win = initialize_window();
	bool should_close = false;
	glClearColor(0.2f, 0.1f, 0.5f, 1.0f);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOther(win, true);
	ImGui_ImplOpenGL3_Init("#version 450");

	bool open_demo = true;

	unsigned int buf;
	glGenBuffers(1, &buf);



#if 0
	while (!should_close) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(win)) {
			should_close = true;
		}


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow(&open_demo);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	
		glfwSwapBuffers(win);
	}
#endif

	return 0;
}