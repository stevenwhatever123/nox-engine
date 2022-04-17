#include <PositionComponent.h>

using namespace NoxEngine;

PositionComponent::PositionComponent(f32 newx, f32 newy, f32 newz) {
	id = 1;
	x = newx;
	y = newy;
	z = newz;
}

void* PositionComponent::CastType(const i32 castID)
{
	switch (castID)
	{
		case 1: case 2:
			return this;
		default: return nullptr;
	}
}
