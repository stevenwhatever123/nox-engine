#include "imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

#include "imguizmo/ImGuizmo.h"
#include "imguizmo/ImSequencer.h"
#include "imguizmo/ImCurveEdit.h"
#include "imguizmo/GraphEditor.h"

#include "MeshScene.h"
#include "MeshNode2.h"

#include <math.h>
#include <vector>
#include <algorithm>

static const char* SequencerItemTypeNames[] = { "Animation" };

using NoxEngine::MeshScene;

struct MySequence : public ImSequencer::SequenceInterface
{
    MySequence(MeshScene* scene);

    MySequence();

    MeshScene* scene;
    int mFrameMin, mFrameMax;

    int GetFrameMin() const;
    int GetFrameMax() const;
    int GetItemCount() const;

    int GetItemTypeCount() const;
    const char* GetItemTypeName(int typeIndex) const;
    const char* GetItemLabel(int index) const;

    void Get(int index, int** start, int** end, int* type, unsigned int* color);
    void Add(int type);
    void Del(int index);
    void Duplicate(int index);

    size_t GetCustomHeight(int index);

    void DoubleClick(int index);

    //void CustomDraw(int index, ImDrawList* draw_list,
    //    const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect,
    //    const ImRect& legendClippingRect);


    //void CustomDrawCompact(int index, ImDrawList* draw_list,
    //    const ImRect& rc, const ImRect& clippingRect);

    // return true if selection is made
    bool Sequencer(SequenceInterface* sequence, int* currentFrame, bool* expanded, int* selectedEntry, int* firstFrame, int sequenceOptions);
};