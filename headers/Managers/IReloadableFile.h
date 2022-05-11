#pragma once

#include <Windows.h>
#include <Core/Types.h>

struct LiveReloadEntry;

namespace NoxEngine {
	class IReloadableFile;

	struct LiveReloadEntry {
		bool changed;
		FILETIME last_write;
		Array<IReloadableFile*> inform_objects;
	};


	
	class IReloadableFile {
		public:
			virtual void liveReloadFile(const char *file_name, LiveReloadEntry* entry) = 0;
	};
}
