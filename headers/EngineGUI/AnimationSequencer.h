#include <imgui/imgui.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

#include <imguizmo/ImGuizmo.h>
#include <imguizmo/ImSequencer.h>
#include <imguizmo/ImCurveEdit.h>
#include <imguizmo/GraphEditor.h>

#include <Core/MeshScene.h>
#include <Core/MeshNode.h>

// #include <math.h>
// #include <vector>
// #include <algorithm>

using NoxEngine::MeshScene;

struct AnimationSequencer : public ImSequencer::SequenceInterface {

    AnimationSequencer(MeshScene* scene);
    AnimationSequencer();

    MeshScene* scene;
    int mFrameMin, mFrameMax;
    bool focused = false;

    int GetFrameMin() const { return 0; }
    int GetFrameMax() const { return scene->numTicks[scene->animationIndex] - 1; }
    int GetItemCount() const { return (i32)scene->allNodes.size(); }

    void BeginEdit(int index) {};
    void EndEdit() {};

    int GetItemTypeCount() const;
    const char* GetItemTypeName(int index) const;
    const char* GetItemLabel(int index) const;
    const char* GetCollapseFmt() const { return "Go Away"; }

    void Get(int index, int** start, int** end, int* type, unsigned int* color);
    void Add(int index) {}
    void Del(int index) {}
    void Duplicate(int index) {}
    void Copy() {}
    void Paste() {}

    size_t GetCustomHeight(int index);
    void DoubleClick(int index) {}
    void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect,
    const ImRect& clippingRect, const ImRect& legendClippingRect);
};
