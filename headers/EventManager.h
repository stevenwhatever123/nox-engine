#pragma once

#include <Singleton.h>
#include <Types.h>

#include <map>
#include <string>
#include <vector>
#include <functional>

#define SIGNAL_EVENT(name, ...) EventManager::Instance()->signal(name, __VA_ARGS__)

namespace NoxEngine {

	typedef std::function<void(va_list)> ListenFunc;
	typedef std::map<std::string, std::vector<ListenFunc>> EventEntry;
	typedef std::map<std::string, std::vector<ListenFunc>>::iterator EventEntryIt;
	typedef std::map<std::string, std::vector<ListenFunc>>::reference EventEntryRef;


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
