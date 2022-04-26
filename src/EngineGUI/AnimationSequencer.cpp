#include <EngineGUI/AnimationSequencer.h>

AnimationSequencer::AnimationSequencer(MeshScene* scene) :
mFrameMin(0), 
mFrameMax(scene->numTicks[scene->animationIndex] - 1),
scene(scene)
{
   
}

AnimationSequencer::AnimationSequencer() :mFrameMin(0), mFrameMax(0) {};

int AnimationSequencer::GetItemTypeCount() const 
{
    return scene->allNodes.size();
}

const char* AnimationSequencer::GetItemTypeName(int index) const
{
    //return "Animation";
    return scene->allNodes[index]->name.c_str();
}

const char* AnimationSequencer::GetItemLabel(int index) const
{
    //return "Animation"; 
    return scene->allNodes[index]->name.c_str();
}

void AnimationSequencer::Get(int index, int** start, int** end, int* type, unsigned int* color)
{
    // Every animation is starting at 0
    i32 items[] = { 0 };

    i32 items2[] = { scene->numTicks[scene->animationIndex] - 1 };

    if (!scene->allNodes[index]->hasAnimations())
    {
        items2[0] = 0;
    }

    // I don't know why this code didn't work
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

size_t AnimationSequencer::GetCustomHeight(int index)
{
    return 0;
    // Every slot has to be the same size otherwise the bar will not be displayed
    //if (scene->allNodes[index]->hasAnimations())
    //    return 300;

    //return 0; 
}

void AnimationSequencer::CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect,
    const ImRect& clippingRect, const ImRect& legendClippingRect)
{
    //static const char* labels[] = { "Translation", "Rotation", "Scale" };

    //bool hasAnimation = scene->allNodes[index]->hasAnimations();

    //if (hasAnimation)
    //{
    //    draw_list->PushClipRect(legendRect.Min, legendRect.Max, true);
    //    for (int i = 0; i < 3; i++)
    //    {
    //        ImVec2 pta(legendRect.Min.x + 60, legendRect.Min.y + i * 20.0f);
    //        // Label
    //        draw_list->AddText(pta,
    //            scene->allNodes[index]->hasAnimations() ? 0xFFFFFFFF : 0x80FFFFFF, labels[i]);
    //    }
    //    draw_list->PopClipRect();

    //    draw_list->PushClipRect(clippingRect.Min, clippingRect.Max, true);
    //    for (int i = 0; i < 3; i++)
    //    {
    //        // The bar
    //        draw_list->AddRectFilled(ImVec2(rc.Min.x - 1, (rc.Min.y + 2) + i * 20.0f),
    //            ImVec2(rc.Max.x - 1, (rc.Min.y - 2) + (i + 1) * 20.0f), 0xFF80FF80, 4);
    //    }
    //    draw_list->PopClipRect();
    //}
}