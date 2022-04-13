#include "EventManager.h"
#include <assert.h>
#include <Types.h>
#include <Utils.h>

using namespace NoxEngine;

void EventManager::addListener(std::string eventName, ListenFunc func) {
	if(_event_subs.contains(eventName)) {
		EventEntryIt entry = _event_subs.find(eventName);
		entry->second.push_back(func);
	} else {
		_event_subs.insert(std::pair(eventName, std::vector<ListenFunc>{func}));
	}
}

void EventManager::signal(std::string eventName) {
	bool exists = _event_subs.contains(eventName);

	assert(exists);

	if(exists) {
		auto it = _event_subs[eventName];
		for(i32 i = 0; i < it.size(); i++) {
			it[i]();
		}
	} else {
		NoxEngineUtils::Logger::debug("%s event doesn't exist, can't signal.", eventName.c_str());
	}
}

