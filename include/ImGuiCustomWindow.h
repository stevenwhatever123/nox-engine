#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Mesh.h"

namespace NoxEngine {
	class ImGuiCustomWindow
	{
		public:
			ImGuiCustomWindow();
			~ImGuiCustomWindow();
		public:
			void showFBXLoaderMenu(Mesh *curMesh);
	};
}
