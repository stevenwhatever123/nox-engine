#include "EngineGUI//ImGuiWidgets.h"

#include <imgui/imgui_internal.h>
#include <imgui/imgui.h>

using namespace NoxEngineGUI;


namespace ImGui {

    // (Vincent) Reference: Code taken from ocornut https://github.com/ocornut/imgui/issues/2718
    SelectableInputResult SelectableInput(const char* str_id, bool selected, ImGuiSelectableFlags flags, char* buf, size_t buf_size) {
        
        SelectableInputResult ret;

        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImVec2 pos_before = window->DC.CursorPos;

        PushID(str_id);
        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(g.Style.ItemSpacing.x, g.Style.FramePadding.y * 2.0f));
        ret.selected = Selectable("##Selectable", selected, flags | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowItemOverlap);
        PopStyleVar();

        ImGuiID id = window->GetID("##Input");
        ret.tempInputActive = TempInputIsActive(id);
        ret.tempInputStart = ret.selected ? IsMouseDoubleClicked(0) : false;

        if (ret.tempInputStart)
            SetActiveID(id, window);

        if (ret.tempInputActive || ret.tempInputStart)
        {
            ImVec2 pos_after = window->DC.CursorPos;
            window->DC.CursorPos = pos_before;
            ret.selected = TempInputText(g.LastItemData.Rect, id, "##Input", buf, (int)buf_size, ImGuiInputTextFlags_None);
            window->DC.CursorPos = pos_after;
        }
        else
        {
            window->DrawList->AddText(pos_before, GetColorU32(ImGuiCol_Text), buf);
        }

        PopID();
        return ret;
    }
}