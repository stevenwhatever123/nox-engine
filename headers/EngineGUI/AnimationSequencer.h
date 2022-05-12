#include <imgui/imgui.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

#include <imguizmo/ImGuizmo.h>
#include <imguizmo/ImSequencer.h>
#include <imguizmo/ImCurveEdit.h>
#include <imguizmo/GraphEditor.h>

#include <Core/MeshScene.h>
#include <Core/MeshNode.h>
#include <Core/GameState.h>
#include <Core/Entity.h>
#include <Components/AnimationComponent.h>

// #include <math.h>
// #include <vector>
// #include <algorithm>

using NoxEngine::MeshScene;
using NoxEngine::GameState;

struct AnimationSequencer : public ImSequencer::SequenceInterface {

    AnimationSequencer(NoxEngine::AnimationComponent *animComp, NoxEngine::GameState* game_state);

    AnimationSequencer();

    NoxEngine::AnimationComponent* animComp;
    GameState* game_state;

    int mFrameMin, mFrameMax;
    bool focused = false;

    int GetFrameMin() const { return 0; }
    int GetFrameMax() const { return animComp->numTicks[animComp->animationIndex] - 1; }
    int GetItemCount() const 
    { 
        //return (i32)animComp->allNodes.size() + (i32) game_state->selectedAudio.size();
        return 1;
    }

    void BeginEdit(int index) {};
    void EndEdit() {};

    int GetItemTypeCount() const;
    const char* GetItemTypeName(int index) const;
    const char* GetItemLabel(int index) const;
    const char* GetCollapseFmt() const { return "Go Away"; }

    void Get(int index, int** start, int** end, int* type, unsigned int* color);

    void Add(int index) 
    {
        if (!game_state->audioSources.empty())
        {
            auto startItr = game_state->audioSources.begin();
            auto endItr = game_state->audioSources.end();
            for (i32 i = 0; i < index; i++)
            {
                startItr++;
            }
            game_state->selectedAudio.push_back(startItr->second.name);
        }
    }

    void Del(int index) {}
    void Duplicate(int index) {}
    void Copy() {}
    void Paste() {}

    size_t GetCustomHeight(int index);
    void DoubleClick(int index) {}
    void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect,
    const ImRect& clippingRect, const ImRect& legendClippingRect);
};
