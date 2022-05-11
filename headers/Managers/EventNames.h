#pragma once

namespace NoxEngine {

	namespace EventNames
	{
#define ADD_EVENT(event_name) static const char* event_name = #event_name;

		ADD_EVENT(meshAdded);
		ADD_EVENT(audioSourceLoaded);
		ADD_EVENT(audioGeometryLoaded);
		ADD_EVENT(createAudioGeometry);
		ADD_EVENT(componentAdded);
		ADD_EVENT(componentRemoved);

#undef ADD_EVENT
	};
}
