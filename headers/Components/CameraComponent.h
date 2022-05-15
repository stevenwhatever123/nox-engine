#pragma once

#include "IComponent.h"
#include <Core/Camera.h>

namespace NoxEngine {
	/*
	 * A mock up class for Renderer to render a cube
	 */
	class CameraComponent : public IComponent {
		public:
			static const ComponentType id = ComponentType::CameraType;

			CameraComponent();
			CameraComponent(const CameraComponent&);
			~CameraComponent();


			f32 get_x() const { return cam.pos.x; }
			f32 get_y() const { return cam.pos.y; }
			f32 get_z() const { return cam.pos.z; }

			void set_x(f32 value) { cam.pos.x = value; }
			void set_y(f32 value) { cam.pos.y = value; }
			void set_z(f32 value) { cam.pos.z = value; }


			inline Camera* getCamera() { return &cam; }
			inline const Camera& getCameraConst() const { return cam; }

		private:
			Camera cam;

	};
}
