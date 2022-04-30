#include <PositionComponent.h>

using namespace NoxEngine;

PositionComponent::PositionComponent(f32 newx, f32 newy, f32 newz)
{
	id = ComponentType::PositionType;
	x = newx;
	y = newy;
	z = newz;
}