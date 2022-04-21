#pragma once

namespace NoxEngine {

	// Note (Vincent): This implies the developer cannot create new component types
	enum ComponentType {
		AbstractType = 0,
		PositionType,	// TODO: change to Transform (pos, rot, scale)
		RenderableType,
		PointLightType,
		AreaLightType,
		AudioSourceType,
		ComponentTypeCount
	};
}