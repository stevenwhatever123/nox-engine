#pragma once

// Core GUI
#include "EngineGUI.h"

// ImGuizmo classes
#include <imguizmo/ImGuizmo.h>


namespace NoxEngine {

	namespace EngineGUI {

		struct GuizmoParams {

		};

		static GuizmoParams guizmoParams;

		// TODO: clean up the static mess
		static bool useWindow = true;
		static int gizmoCount = 1;
		static float camDistance = 8.f;
		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
		static bool useSnap = false;
		static float snap[3] = { 1.f, 1.f, 1.f };
		static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
		static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
		static bool boundSizing = false;
		static bool boundSizingSnap = false;

		static float objectMatrix[4][16] = {
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

		static float cameraView[16] =
		{ 1.f, 0.f, 0.f, 0.f,
		  0.f, 1.f, 0.f, 0.f,
		  0.f, 0.f, 1.f, 0.f,
		  0.f, 0.f, 0.f, 1.f };

		static float cameraProjection[16];


		static int lastUsing = 0;

		// Camera projection
		static bool isPerspective = true;
		static float fov = 27.f;	// TODO: reuse in glm::perspective (Renderer)
		static float viewWidth = 10.f; // for orthographic
		// TODO: ignore, use the existing values in glm::lookAt (camera)
		static float camYAngle = 165.f / 180.f * 3.14159f;
		static float camXAngle = 32.f / 180.f * 3.14159f;

		void updateImGuizmoDemo(const GUIParams* params);

	}

}