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

using NoxEngine::MeshScene;

struct TestSequencer : public ImSequencer::SequenceInterface {

    TestSequencer(MeshScene* scene) :mFrameMin(0), mFrameMax(scene->numTicks[scene->animationIndex] - 1)
    {
        this->scene = scene;
    }

    TestSequencer() :mFrameMin(0), mFrameMax(0) {};

    MeshScene* scene;
    int mFrameMin, mFrameMax;

    bool focused = false;

    int GetFrameMin() const { return 0; }
    int GetFrameMax() const { return scene->numTicks[scene->animationIndex] - 1; }
    int GetItemCount() const { return scene->allNodes.size(); }

    void BeginEdit(int index) 
    {
        
    }

    void EndEdit() {

    }

    int GetItemTypeCount() const {
        return scene->allNodes.size();
    }

    const char* GetItemTypeName(int index) const {
        //return "Animation";
        return scene->allNodes[index]->name.c_str();
    }

    const char* GetItemLabel(int index) const 
    { 
        //return "Animation"; 
        return scene->allNodes[index]->name.c_str();
    }
    const char* GetCollapseFmt() const { return "Go Away"; }

    void Get(int index, int** start, int** end, int* type, unsigned int* color) 
    {
        // Every animation is starting at 0
        i32 items[] = { 0 };

        i32 items2[] = { scene->numTicks[scene->animationIndex] - 1};

        if (!scene->allNodes[index]->hasAnimations())
        {
            items2[0] = 0;
        }

        //i32 items2[] = { (scene->allNodes[index]->hasAnimations() ? scene->numTicks[scene->animationIndex] - 1 : 1) };

        if (color)
            *color = 0xFFAA8080; // same color for everyone, return color based on type
        if (start)
            *start = items;
        if (end)
            *end = items2;
        if (type)
            *type = index;
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