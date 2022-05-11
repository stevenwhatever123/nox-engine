#define IMGUI_DEFINE_MATH_OPERATORS     // might not be the best idea

#include <EngineGUI/ImGuiWidgets.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui.h>

using namespace NoxEngineGUI;


namespace ImGui {

    bool ImGui::ImageButtonWithText(ImTextureID texId, const char* label, const ImVec2& imageSize, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col) {
        
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        int width = ImGui::GetContentRegionAvail().x;

        ImVec2 size = imageSize;
        if (size.x <= 0 && size.y <= 0) { size.x = size.y = ImGui::GetTextLineHeightWithSpacing(); }
        else {
            if (size.x <= 0)          size.x = size.y;
            else if (size.y <= 0)     size.y = size.x;
            size *= window->FontWindowScale * ImGui::GetIO().FontGlobalScale;
        }

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        const ImGuiID id = window->GetID(label);
        ImVec2 textSize = ImGui::CalcTextSize(label, NULL, true);
        textSize.x = ImGui::GetContentRegionAvail().x;
        const bool hasText = textSize.x > 0;

        const float innerSpacing = hasText ? ((frame_padding >= 0) ? (float)frame_padding : (style.ItemInnerSpacing.x)) : 0.f;
        const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
        const ImVec2 totalSizeWithoutPadding(size.x + innerSpacing + textSize.x, size.y > textSize.y ? size.y : textSize.y);
        const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + totalSizeWithoutPadding + padding * 2);
        
        ImVec2 start(0, 0);
        start = window->DC.CursorPos + padding; if (size.y < textSize.y) start.y += (textSize.y - size.y) * .5f;
        
        const ImRect image_bb(start, start + size);
        start = window->DC.CursorPos + padding; start.x += size.x + innerSpacing; if (size.y > textSize.y) start.y += (size.y - textSize.y) * .5f;
        ImGui::ItemSize(bb);
        if (!ImGui::ItemAdd(bb, id))
            return false;

        bool hovered = false, held = false;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

        // Render
        const ImU32 col = ImGui::GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        ImGui::RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
        if (bg_col.w > 0.0f)
            window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, ImGui::GetColorU32(bg_col));

        window->DrawList->AddImage(texId, image_bb.Min, image_bb.Max, uv0, uv1, ImGui::GetColorU32(tint_col));

        if (textSize.x > 0) ImGui::RenderText(start, label);
        return pressed;
    }

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
