#include <Managers/LiveReloadManager.h>
#include <Utils/Utils.h>

using namespace NoxEngine;

void LiveReloadManager::addLiveReloadEntry(const char *file_name, IReloadableFile *informed_object) {

	auto itr = reload_files.find(file_name);

	if(itr != reload_files.end()) {
		itr->second.inform_objects.push_back(informed_object);
	} else {
		auto emplaced_item = reload_files.emplace(file_name, LiveReloadEntry{});
		if(emplaced_item.second) {
			emplaced_item.first->second.last_write = NoxEngineUtils::FileUtils::getLastWriteTime(file_name);
			emplaced_item.first->second.inform_objects.push_back(informed_object);
		}
	}

}

void LiveReloadManager::removeLiveReloadEntry(const char *file_name, IReloadableFile *informed_object) {

	auto itr = reload_files.find(file_name);

	if(itr != reload_files.end()) {

		auto& informed_objects = itr->second.inform_objects;


		auto found = std::find(informed_objects.begin(), informed_objects.end(), informed_object);

		if(found != informed_objects.end()) {
			informed_objects.erase(found);

			if(informed_objects.size() == 0) {
				reload_files.erase(itr);
			}

		}
	}

}


void LiveReloadManager::checkFiles() {
	auto file = reload_files.begin();
	for(;file != reload_files.end(); file++) {
		if(file->second.changed) continue;
		FILETIME currentTimeStamp = NoxEngineUtils::FileUtils::getLastWriteTime(file->first);
		if(CompareFileTime(&file->second.last_write, &currentTimeStamp) == -1) {
			file->second.changed = 1;
			for(auto infObj = file->second.inform_objects.begin(); infObj != file->second.inform_objects.end(); infObj++) {
				infObj[0]->liveReloadFile(file->first, &file->second);
			}
		}
	}

}

