#include <Components/TransformComponent.h>

using namespace NoxEngine;

TransformComponent::TransformComponent(f32 newx, f32 newy, f32 newz)
{
	//id = ComponentType::TransformType;
	x = newx;
	y = newy;
	z = newz;

	rx = 0;
	ry = 0;
	rz = 0;

	sx = 1.0f;
	sy = 1.0f;
	sz = 1.0f;
}