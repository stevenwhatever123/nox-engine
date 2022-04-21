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

struct TestSequencer : public ImSequencer::SequenceInterface {

    TestSequencer() :mFrameMin(0), mFrameMax(0) {};

    bool focused = false;
    int GetFrameMin() const { return 0; }
    int GetFrameMax() const { return 100; }
    int GetItemCount() const { return 1; }

    int mFrameMin, mFrameMax;

    void BeginEdit(int index) {
    }

    void EndEdit() {

    }

    int GetItemTypeCount() const {
        return 1;
    }
    const char* GetItemTypeName(int index) const {
        return "Animation";
    }

    const char* GetItemLabel(int index) const { return "Animation"; }
    const char* GetCollapseFmt() const { return "Go Away"; }

    void Get(int index, int** start, int** end, int* type, unsigned int* color) {

        static int items[] = { 0, 1, 2, 3, 4, 5, 6 };
        static int items2[] = { 7, 8, 9, 10, 11, 12, 13 };

        if (color)
            *color = 0xFFAA8080; // same color for everyone, return color based on type
        if (start)
            *start = items;
        if (end)
            *end = items2;
        if (type)
            *type = 1;

    }
    void Add(int index) {}
    void Del(int index) {}
    void Duplicate(int index) {
    }

    void Copy() {}
    void Paste() {}

    size_t GetCustomHeight(int index) { return 0; }
    void DoubleClick(int index) {}

};