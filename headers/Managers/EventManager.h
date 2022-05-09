#pragma once

#include "Singleton.h"
#include <Core/Types.h>

#include <map>
#include <functional>

namespace NoxEngine {

	typedef std::function<void(va_list)> ListenFunc;
	typedef std::map<String, Array<ListenFunc>> EventEntry;
	typedef std::map<String, Array<ListenFunc>>::iterator EventEntryIt;
	typedef std::map<String, Array<ListenFunc>>::reference EventEntryRef;

	class EventManager: public Singleton<EventManager> {
		friend class Singleton<EventManager>;

		public:

		void addListener(String eventName, ListenFunc func);
		void signal(String eventName, ...) ;

		protected:

			EventManager(){}
			~EventManager(){}

		private:
			EventEntry _event_subs;
	};

}

#define SIGNAL_EVENT(name, ...) EventManager::Instance()->signal(name, __VA_ARGS__)
