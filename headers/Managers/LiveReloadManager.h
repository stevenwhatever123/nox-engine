#pragma once


#include "Singleton.h"
#include <Core/Types.h>
#include <Managers/IReloadableFile.h>

namespace NoxEngine {

	class LiveReloadManager : public Singleton<LiveReloadManager> {
		friend class Singleton<LiveReloadManager>;
		public:
			// void addLiveReloadEntry(LiveReloadEntry entry);
			void addLiveReloadEntry(const char *file_name, IReloadableFile *obj);
			void removeLiveReloadEntry(const char *file_name, IReloadableFile *obj);
			void checkFiles();

		private:
			Map<const char*, LiveReloadEntry> reload_files;

	};

}
