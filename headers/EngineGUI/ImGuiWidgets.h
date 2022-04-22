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

    // (Vincent) Reference: Code taken from ocornut https://github.com/ocornut/imgui/issues/2718
    NoxEngineGUI::SelectableInputResult SelectableInput(const char* str_id, bool selected, ImGuiSelectableFlags flags, char* buf, size_t buf_size);
}