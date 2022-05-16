#include <EngineGUI/ScenePanel.h>
#include <Core/Entity.h>
#include <EngineGUI/EngineGUI.h>
#include <EngineGUI/PresetObject.h>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

using namespace NoxEngine;

void NoxEngineGUI::updateScenePanel(GameState* state, GUIParams *params) {

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
			// On button release, get cursor position
			ImGuiIO& io = ImGui::GetIO();
			ImVec2 origin = ImGui::GetCursorScreenPos(); // Start pos of this window relative to the whole app window
			const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y); // MousePos is mouse position relative to the whole window.

			// Transform cursor position to the world position with depth set to 5 (arbitary)
			glm::vec3 mouse_pos_in_world = getPosOfMouseInWorldCoord(
				state, mouse_pos_in_canvas.x,
				mouse_pos_in_canvas.y,
				ImGui::GetWindowWidth(),
				ImGui::GetWindowHeight()
			);

			// Create entity
			Entity* ent = new Entity(state->activeScene, PRESET_OBJECT_NAMES_MAP[payloadObject].c_str());

			for (auto it = PRESET_OBJECT_COMPONENTS[payloadObject].begin();
				it != PRESET_OBJECT_COMPONENTS[payloadObject].end(); it++)
			{
				ComponentType type = it->first;
				IComponent* comp = it->second;

				if (type == TransformType)
				{
					// Create position of the entity based on the world pos of the cursor
					TransformComponent* pos = new TransformComponent(mouse_pos_in_world.x, mouse_pos_in_world.y, mouse_pos_in_world.z);
				
					ent->addComp(pos);
				}

				if (type == RenderableType)
				{
					RenderableComponent* rendComp 
						= new RenderableComponent( *(RenderableComponent*)comp);

					ent->addComp(rendComp);
				}
			}

			// Add an entity to the active scene
			state->activeScene->addEntity(ent);
		}
		ImGui::EndDragDropTarget();
	}

	ImVec2 wsize = ImGui::GetContentRegionAvail();


	// LOG_DEBUG("b: cur: (%d %d), prev: (%d %d), new: (%f, %f)",
	// 	state->win_width, state->win_height,
	// 	state->prev_win_width, state->prev_win_height,
	// 	wsize.x, wsize.y
	// );

	if(params->full_screen == false) {

		state->prev_win_width  = state->win_width;
		state->prev_win_height = state->win_height;

		state->win_width  = (u32)wsize.x;
		state->win_height = (u32)wsize.y;
	}



	state->renderer->updateProjection((i32)wsize.x, (i32)wsize.y);

	if(ImGui::IsWindowFocused()) {
		params->full_screen = false;
		params->scene_active = true;


		// LOG_DEBUG("a: cur: (%d %d), prev: (%d %d), new: (%f, %f)",
		// 	state->win_width, state->win_height,
		// 	state->prev_win_width, state->prev_win_height,
		// 	wsize.x, wsize.y
		// );



	} else {
		params->scene_active = false;
	}


	ImGui::Image((ImTextureID)(u64)state->renderer->getTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();

	if (params->selectedEntity < state->activeScene->entities.size())
	{

		Entity* ent = state->activeScene->entities[params->selectedEntity];

		if (ent != nullptr)
		{
			// Set viewport
			ImGuizmo::SetRect(
				ImGui::GetWindowPos().x,
				ImGui::GetWindowPos().y,
				(f32)state->renderer->getWidth(),
				(f32)state->renderer->getHeight()
			);

			// Camera
			glm::mat4 cameraProjection = state->renderer->getProjMatr();
			glm::mat4 cameraView = state->renderer->getCameraMatr();


			// Object transform
			glm::mat4 worldMat = glm::mat4(1.0f);
			glm::mat4 translation(0);
			glm::mat4 rotation(0);
			glm::mat4 scale(1);

			bool hasTransformAndisEnabled = ent->containsComps<TransformComponent>() && ent->isEnabled<TransformComponent>();
			bool hasAnimationCompAndIsEnabled = ent->containsComps<AnimationComponent>() && ent->isEnabled<AnimationComponent>();

			AnimationComponent* animComp = nullptr;

			if (hasTransformAndisEnabled) {
				if (hasAnimationCompAndIsEnabled)
				{
					animComp = ent->getComp<AnimationComponent>();
				}

				if (animComp != nullptr && animComp->editing)
				{
					translation = animComp->translationMatrices[animComp->animationIndex][animComp->frameIndex];
					rotation = animComp->rotationMatrices[animComp->animationIndex][animComp->frameIndex];
					scale = animComp->scalingMatrices[animComp->animationIndex][animComp->frameIndex];
					worldMat = translation * rotation * scale;
				}
				else
				{
					ITransform* pos = ent->getComp<TransformComponent>()->CastType<ITransform>();
					translation = glm::translate(glm::mat4(1.0f), glm::vec3(pos->x, pos->y, pos->z));
					rotation = glm::eulerAngleXYZ(pos->rx, pos->ry, pos->rz);
					scale = glm::scale(glm::mat4(1.0f), glm::vec3(pos->sx, pos->sy, pos->sz));
					worldMat = translation * rotation * scale;
				}
			}

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)params->imguizmoMode, ImGuizmo::LOCAL, glm::value_ptr(worldMat));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 scale_temp;
				glm::quat rotation_temp;
				glm::vec3 translation_temp;
				glm::vec3 skew_temp;
				glm::vec4 perspective_temp;
				glm::decompose(worldMat, scale_temp, rotation_temp, translation_temp, skew_temp, perspective_temp);

				if (hasTransformAndisEnabled)
				{
					if (animComp != nullptr && animComp->editing)
					{
						animComp->translationMatrices[animComp->animationIndex][animComp->frameIndex][3][0] = translation_temp[0];
						animComp->translationMatrices[animComp->animationIndex][animComp->frameIndex][3][1] = translation_temp[1];
						animComp->translationMatrices[animComp->animationIndex][animComp->frameIndex][3][2] = translation_temp[2];

						glm::extractEulerAngleXYZ(glm::toMat4(rotation_temp), 
							animComp->eulerAngleXYZ[animComp->animationIndex][animComp->frameIndex][0], 
							animComp->eulerAngleXYZ[animComp->animationIndex][animComp->frameIndex][1], 
							animComp->eulerAngleXYZ[animComp->animationIndex][animComp->frameIndex][2]);

						animComp->scalingMatrices[animComp->animationIndex][animComp->frameIndex][0][0] = scale_temp[0];
						animComp->scalingMatrices[animComp->animationIndex][animComp->frameIndex][1][1] = scale_temp[1];
						animComp->scalingMatrices[animComp->animationIndex][animComp->frameIndex][2][2] = scale_temp[2];
					
						animComp->updateTransformation();
					}
					else
					{
						ITransform* pos = ent->getComp<TransformComponent>()->CastType<ITransform>();
						pos->x = translation_temp[0];
						pos->y = translation_temp[1];
						pos->z = translation_temp[2];

						glm::extractEulerAngleXYZ(glm::toMat4(rotation_temp), pos->rx, pos->ry, pos->rz);

						pos->sx = scale_temp[0];
						pos->sy = scale_temp[1];
						pos->sz = scale_temp[2];
					}
				}
			}
		}
	}

	// Window end
	ImGui::End();
	ImGui::PopStyleVar();
}

// A function transformitng screen space pixel coord of the point to the world coord
// Param: point coordinates in pixels relative to the window it is in. Width and Height of the window
glm::vec3 NoxEngineGUI::getPosOfMouseInWorldCoord(GameState* params, f32 pointX, f32 pointY, f32 width, f32 height)
{
	// x and y are coordinates of the pixel in the rendering window

	// Convert x and y to 3d Normalised Device Coordinates
	// The point lies on the "screen", the near plane of the normalised cube
	glm::vec2 clickedPoint = glm::vec2((2.0f * pointX) / width - 1.0f, 1.0f - (2.0f * pointY) / height);


	// Each point on the screen corresponds to a line in the 3d space
	// To find that line define 2 points it goes through
	glm::vec4 from(clickedPoint.x, clickedPoint.y, -1.0f, 1.0f);
	glm::vec4 to(clickedPoint.x, clickedPoint.y, 1.0f, 1.0f);

	// Multiply by reverse projection
	from = glm::inverse(params->renderer->getProjMatr()) * from;
	to = glm::inverse(params->renderer->getProjMatr()) * to;

	from /= from.w;
	to /= to.w;

	from = glm::inverse(params->renderer->getCameraMatr()) * from;
	to = glm::inverse(params->renderer->getCameraMatr()) * to;

	glm::vec4 direction = to - from;
	direction = glm::normalize(direction);

	// Now we have a line in the world space coord, that is the original 2d point

	// Pick prameter t (how far away from the camera the point will be)
	// ASK THE TEAM HOW THEY WANT IT IMPLEMENTED
	// For now just pick 5
	float t = 10.0f;

	// Knowing updated t and the line, find the new point in the world coord
	glm::vec4 new_coord = from + t * direction;

	return glm::vec3(new_coord.x, new_coord.y, new_coord.z);
}
