#include <EngineGUI/FullscreenShaderPanel.h>
#include <FullscreenShader.h>
#include <Core/Entity.h>
#include <EngineGUI/PresetObject.h>
#include <Managers/IOManager.h>

using namespace NoxEngine;

void NoxEngineGUI::updateFullscreenShaderPanel(GameState* state, GUIParams *ui_params) {

	String name = kPanelNameMap[ PanelName::FullscreenShader ];

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	
	ImGui::Begin(name.c_str(), NULL, flags);

	

	ImVec2 wsize = ImGui::GetContentRegionAvail();
	if(ImGui::IsWindowFocused()) {
		ui_params->full_screen = true;
		
	}


	if(ui_params->full_screen) {
		state->prev_win_width  = state->win_width;
		state->prev_win_height = state->win_height;

		state->win_width  = (u32)wsize.x;
		state->win_height = (u32)wsize.y;
	}


	
	state->renderer->updateProjection((i32)wsize.x, (i32)wsize.y);
	ImGui::Image((ImTextureID)(u64)state->fullscreen_shader_texture_used, wsize, ImVec2(0, 1), ImVec2(1, 0));

	// Window end
	ImGui::End();
	ImGui::PopStyleVar();
}

f32 padding_right = 5.0f;
f32 ratio = 16.0f/9.0f;
char temp_buf[256];

void TextureView(const char *tree_name, const char *file_path, FullscreenShader& pp, Array<FullscreenShader>& pps, u32 main_renderer_tex_id, GameState *state) {

	if(ImGui::TreeNodeEx(pp.GetName(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen, "%s", pp.GetName())) {

		ImGui::Unindent();
		ImGui::Indent(padding_right);

		ImGui::Text("Texture ID: %d", pp.GetTexture());

		bool openInputs = ImGui::TreeNode("Inputs");

		ImGui::SameLine();
		if(ImGui::SmallButton("+")) {
			ImGui::OpenPopup("Texture Inputs");
		}

		ImGui::SetNextWindowSize({550, 250});
		if(ImGui::BeginPopupModal("Texture Inputs", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {


			static i32 selected_texture = 0;

			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth() - ImGui::CalcTextSize("Available Inputs").x - 30.0f);

			if(selected_texture == 0) {
				snprintf(temp_buf, 256, "Tex: Main Renderer, TexID: %d", main_renderer_tex_id);
			} else {
				snprintf(temp_buf, 256, "Tex: %s, TexID: %d", pps[selected_texture - 1].GetName(), pps[selected_texture - 1].GetTexture());
			}

			ImGui::Text("Available Inputs");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth() - ImGui::CalcTextSize("Available Inputs").x - 55.0f);
			if(ImGui::BeginCombo("##Available Inputs", temp_buf)) {

				if(ImGui::Selectable("Main Renderer")) {
					selected_texture = 0;
				}

				for(u32 i = 0; i < pps.size(); i++) {

					if(&pp == &pps[i]) continue;

					ImGui::PushID(i);
					snprintf(temp_buf, 256, "Tex: %s, TexID: %d", pps[i].GetName(), pps[i].GetTexture());
					if(ImGui::Selectable(temp_buf)) {
						selected_texture = i+1;
					}
					ImGui::PopID();
				}
				ImGui::EndCombo();
			}

			static i32 selected_texture_unit = 0;

			ImGui::Text("Texture Units   ");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth() - ImGui::CalcTextSize("Texture Units   ").x - 55.0f);
			snprintf(temp_buf, 256,  "Texture Unit: %d", selected_texture_unit);
			if(ImGui::BeginCombo("##Texture Units", temp_buf)) {

				for(u32 i = GL_TEXTURE0; i <= GL_TEXTURE31; i++) {
					snprintf(temp_buf, 256,  "Texture Unit: %d", i - GL_TEXTURE0);
					if(ImGui::Selectable(temp_buf)) {
						selected_texture_unit = i - GL_TEXTURE0;
					}
				}

				ImGui::EndCombo();
			}


			const char *selected_texture_name = selected_texture == 0 ? "Main Renderer" : pps[selected_texture - 1].GetName();
			ImGui::Text("Selected Texture: %s - ID: %d, Selected Texture Unit: %d", selected_texture_name, selected_texture, selected_texture_unit );

			if(ImGui::Button("Add Input")) {

				if(selected_texture == 0) {
					pp.AddInput(main_renderer_tex_id, selected_texture_unit);
				} else {
					selected_texture -= 1;
					pp.AddInput(pps[selected_texture].GetTexture(), selected_texture_unit);
				}

				selected_texture = 0;
				selected_texture_unit = 0;

				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			if(ImGui::Button("Cancel")) {
				ImGui::CloseCurrentPopup();
			}


			ImGui::EndPopup();
		}

		if(openInputs) {

			auto& inputs = pp.GetTextureInputs();
			i32 item_to_delete = -1;
			for(u32 i = 0; i < inputs.size(); i++) {
				ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth() - 100.0f);
				snprintf(temp_buf, 256, "TexID: %d, Tex Unit: %d", inputs[i].texture_id, inputs[i].texture_location);
				ImGui::InputText("", temp_buf, 256, ImGuiInputTextFlags_ReadOnly);

				if(ImGui::GetActiveID() == ImGui::GetID("")) {
					ImGui::ClearActiveID();
				}


				ImGui::SameLine();
				ImGui::PushID(i);
				ImGui::SetNextItemWidth(70.0f);
				if(ImGui::Button("Remove")) {
					item_to_delete = i;
				}
				ImGui::PopID();
			}

			if(item_to_delete > -1)
				pp.RemoveInput(item_to_delete);

			ImGui::TreePop();
		}

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 70.0f);
		ImGui::InputText("", (char*)pp.GetShader(), 256, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();

		ImGui::SetNextItemWidth(60.0f);
		if(ImGui::Button("Change")) {
			String filename = IOManager::Instance()->PickFile();
			if(!filename.empty()) {
				pp.ChangeShader(filename);
			}
		}

		if(ImGui::GetActiveID() == ImGui::GetID("")) {
			ImGui::ClearActiveID();
		}


		ImVec2 wsize;
		wsize.x = ImGui::GetContentRegionAvail().x;
		wsize.y = wsize.x/ratio;
		if(pp.IsInit())
			ImGui::Image((ImTextureID)(u64)pp.GetTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

		if(ImGui::Button("Preview")) {
			state->fullscreen_shader_texture_used = pp.GetTexture();
		}

		ImGui::TreePop();
		ImGui::Unindent(padding_right);

		ImGui::Indent();
	}
}

void NoxEngineGUI::updatePostProcessorsPanel(GameState* state, GUIParams *ui_params) {
	String name = kPanelNameMap[ PanelName::PostPorcessors ];


	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize;
	ImGui::Begin(name.c_str(), NULL, flags);

	if(ImGui::Button("Add Post Processor")) {
		state->post_processors.emplace_back(state->win_width, state->win_height);
	}

	if(ImGui::TreeNodeEx("Main Renderer", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {
		
		ImGui::Unindent();
		ImGui::Indent(padding_right);

		ImGui::PushID("TextInput");
		if(ImGui::GetActiveID() == ImGui::GetID("")) {
			ImGui::ClearActiveID();
		}
		ImGui::PushItemWidth(-padding_right);
		ImGui::InputText("", (char*)"Main Renderer", 256, ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		ImVec2 wsize;
		wsize.x = ImGui::GetContentRegionAvail().x - padding_right;
		wsize.y = (9.0f/16.0f)*wsize.x;
		ImGui::Image((ImTextureID)(u64)state->renderer->getTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PopID();
		ImGui::TreePop();

		ImGui::Unindent(padding_right);
		ImGui::Indent();

	}

	i32 i = 0;

	for(auto pp = state->post_processors.begin(); pp != state->post_processors.end(); pp++, i++ ) {
		ImGui::PushID(i);
		TextureView(pp->GetName(), pp->GetShader(), *pp, state->post_processors, state->renderer->getTexture(), state);
		ImGui::PopID();
	}

	ImGui::End();

}


