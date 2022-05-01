#pragma once
#include "ScriptsManager.h"
#include <IComponent.h>
#include <IPosition.h>
#include <Types.h>

/*
* Position Component. Handles postion of the the entity in the 3D world
*/

namespace NoxEngine {

	class PositionComponent : public IPosition, public IComponent
	{
		public:
			void* CastType(const i32 castID);
			PositionComponent(f32 newx, f32 newy, f32 newz);
			static void exportLua()
			{
#if 0
				auto lua_state = ScriptsManager::Instance()->get_lua_state();
				luaL_openlibs(lua_state);

				luabridge::getGlobalNamespace(lua_state).
					beginNamespace("game").
					beginClass<IComponent>("IComponent").
					addConstructor<void(*)(void)>().
					addProperty("id", &IComponent::get_id, &IComponent::set_id).
					endClass().
					beginClass<IPosition>("IPosition").
					addConstructor<void(*)(void)>().
					addProperty("x", &IPosition::get_x, &IPosition::set_x).
					addProperty("y", &IPosition::get_y, &IPosition::set_y).
					addProperty("z", &IPosition::get_z, &IPosition::set_z).
					endClass().
					deriveClass <PositionComponent, IPosition>("PositionComponent").
					addConstructor <void (*) (const f32&, const f32&, const f32&)>().
					addProperty("id", &PositionComponent::get_id, &PositionComponent::set_id).
					endClass().
					endNamespace();
			
#endif		
			};
	};


}

