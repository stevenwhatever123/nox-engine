#pragma once

namespace NoxEngine {

	namespace EventNames
	{
#define ADD_EVENT(event_name) static const char* event_name = #event_name;

		ADD_EVENT(meshAdded);

#undef ADD_EVENT
	};
}
