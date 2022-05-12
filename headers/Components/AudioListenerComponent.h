#pragma once

#include "Components/IComponent.h"
#include "Components/IRenderable.h"
#include "Components/IAudioListener.h"
#include "Core/Types.h"

namespace NoxEngine {

	class AudioListenerComponent : public IAudioListener, public IRenderable, public IComponent {

	public:
		static const ComponentType id = ComponentType::AudioListenerType;

		AudioListenerComponent();

		static void exportLua() { };

		virtual Entity* getParentEntity() override { return parent; };
		virtual void attachedToEntity(Entity* ent) override { parent = ent; };

		// setActive and getActive?

		inline const Array<vec3>&  getVertices() const override { return vertices; }
		inline const Array<i32>&   getIndices () const override { return indices; }

		// Unused
		inline const Array<vec2>& getTexCoords() const override { return texCoords; }
		inline const Array<vec3>& getNormals()   const override { return normals; }
		inline const Array<ivec3>& getFaces()	 const override { return faces; }
		inline const i32 getNumOfVertices()      const override { return (i32)vertices.size(); }
		inline const i32 getNumOfFaces()         const override { return (i32)faces.size(); }
		inline const i32 getNumOfTexCoord()      const override { return (i32)texCoords.size(); }
		inline const i32 getNumOfNormals()       const override { return (i32)normals.size(); }
		inline const String getNormalTexture()         override { return ""; }
		inline const String getAmbientTexture()        override { return ""; }

	};
}

