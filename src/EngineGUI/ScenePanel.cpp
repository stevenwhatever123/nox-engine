#include <EngineGUI/ScenePanel.h>
#include <Core/Entity.h>
#include <EngineGUI/EngineGUI.h>
#include <EngineGUI/PresetObject.h>

#include <glm/gtx/matrix_decompose.hpp>

using namespace NoxEngine;

void NoxEngineGUI::updateScenePanel(GameState* state, GUIParams* params) {

	String name = kPanelNameMap[ PanelName::Scene ];

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin(name.c_str(), NULL, flags);

	ImGuiID id;
	ImRect bb = ImGui::GetCurrentWindow()->WorkRect;
	ImGui::GetCurrentWindow()->GetID(&id);

	if (ImGui::BeginDragDropTargetCustom(bb, id)) {
	
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PRESET_OBJECT_DND")) {

			IM_ASSERT(payload->DataSize == sizeof(PresetObject));
	
			PresetObject payloadObject = *(const PresetObject*)payload->Data;
			printf("Dropped %s (%i)\n", PRESET_OBJECT_NAMES_MAP[payloadObject].c_str(), payloadObject);
		
			// TODO: Logic for translating 2D cursor position to 3D world coordinates


			// Add an entity to the active scene
			state->activeScene->addEntity(payloadObject);
		}
		ImGui::EndDragDropTarget();
	}

	ImVec2 wsize = ImGui::GetContentRegionAvail();
	ImGui::Image((ImTextureID)(u64)state->renderer->getTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();

	if (params->selectedEntity < state->activeScene->entities.size())
	{

		Entity* ent = state->activeScene->entities[params->selectedEntity];

		if (ent != nullptr)
		{
			// Set viewport
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, state->renderer->getWidth(), state->renderer->getHeight());

			// Camera
			glm::mat4 cameraProjection = state->renderer->getProjMatr();
			glm::mat4 cameraView = state->renderer->getCameraMatr();


			// Object transform
			glm::mat4 worldMat = glm::mat4(1.0f);
			glm::mat4 translation(0);
			glm::mat4 rotation(0);
			glm::mat4 scale(1);

			bool hasTransformAndisEnabled = ent->containsComps<TransformComponent>() && ent->isEnabled<TransformComponent>();

			if (hasTransformAndisEnabled) {
				ITransform* pos = ent->getComp<TransformComponent>()->CastType<ITransform>();
				translation = glm::translate(glm::mat4(1.0f), glm::vec3(pos->x, pos->y, pos->z));
				rotation = glm::eulerAngleXYZ(pos->rx, pos->ry, pos->rz);
				scale = glm::scale(glm::mat4(1.0f), glm::vec3(pos->sx, pos->sy, pos->sz));
				worldMat = translation * rotation * scale;
			}

			if ((ImGuizmo::OPERATION)params->imguizmoMode == ImGuizmo::OPERATION::TRANSLATE)
			{
				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)params->imguizmoMode, ImGuizmo::LOCAL, glm::value_ptr(translation));
			}
			if ((ImGuizmo::OPERATION)params->imguizmoMode == ImGuizmo::OPERATION::ROTATE)
			{
				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)params->imguizmoMode, ImGuizmo::LOCAL, glm::value_ptr(rotation));
			}
			if ((ImGuizmo::OPERATION)params->imguizmoMode == ImGuizmo::OPERATION::SCALE)
			{
				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)params->imguizmoMode, ImGuizmo::LOCAL, glm::value_ptr(scale));
			}

			if (ImGuizmo::IsUsing())
			{
				// Turns out glm decompose matrix sucks so we don't use it

				if (hasTransformAndisEnabled)
				{
					ITransform* pos = ent->getComp<TransformComponent>()->CastType<ITransform>();
					pos->x = translation[3][0];
					pos->y = translation[3][1];
					pos->z = translation[3][2];

					glm::extractEulerAngleXYZ(rotation, pos->rx, pos->ry, pos->rz);

					pos->sx = scale[0][0];
					pos->sy = scale[1][1];
					pos->sz = scale[2][2];
				}
			}
		}
	}

	// Window end
	ImGui::End();
	ImGui::PopStyleVar();
}


