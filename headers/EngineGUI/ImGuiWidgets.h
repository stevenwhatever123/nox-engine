#pragma once

#include <imgui/imgui.h>

namespace NoxEngineGUI {

    struct SelectableInputResult {
        bool selected : 1;
        bool tempInputActive : 1;
        bool tempInputStart : 1;
    };
}

namespace ImGui {

    bool ImageButtonWithText(ImTextureID texId, const char* label, const ImVec2& imageSize = ImVec2(0, 0), const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));

    // (Vincent) Reference: Code taken from ocornut https://github.com/ocornut/imgui/issues/2718
    NoxEngineGUI::SelectableInputResult SelectableInput(const char* str_id, bool selected, ImGuiSelectableFlags flags, char* buf, size_t buf_size);
}
