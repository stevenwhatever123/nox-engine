#pragma once

// Core GUI
#include "EngineGUI.h"

// ImGuizmo classes
#include <imguizmo/ImGuizmo.h>
#include <Core/GameState.h>
#include <Core/Types.h>

namespace NoxEngineGUI {

	struct GuizmoParams { };

	static GuizmoParams guizmoParams;

	// TODO: clean up the static mess
	static bool useWindow = true;
	static i32 gizmoCount = 1;
	static f32 camDistance = 8.f;
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
	static bool useSnap = false;
	static f32 snap[3] = { 1.f, 1.f, 1.f };
	static f32 bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static f32 boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;

	static f32 objectMatrix[4][16] = {
		{ 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f },

		{ 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		2.f, 0.f, 0.f, 1.f },

		{ 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		2.f, 0.f, 2.f, 1.f },

		{ 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 2.f, 1.f }
	};

	static f32 cameraView[16] =
	{ 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f };

	static f32 cameraProjection[16];


	static i32 lastUsing = 0;

	// Camera projection
	static bool isPerspective = true;
	static f32 fov = 27.f;	// TODO: reuse in glm::perspective (Renderer)
	static f32 viewWidth = 10.f; // for orthographic
	// TODO: ignore, use the existing values in glm::lookAt (camera)
	static f32 camYAngle = 165.f / 180.f * 3.14159f;
	static f32 camXAngle = 32.f / 180.f * 3.14159f;

	void updateImGuizmoDemo(const GUIParams* params);

}
