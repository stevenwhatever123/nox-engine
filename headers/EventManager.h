#ifndef NOX_EVENT_MANAGER_H
#define NOX_EVENT_MANAGER_H

#include "Singleton.h"
#include <map>
#include <string>
#include <vector>
#include <functional>

namespace NoxEngine {

	typedef std::function<void(va_list)> ListenFunc;
	typedef std::map<std::string, std::vector<ListenFunc>> EventEntry;
	typedef std::map<std::string, std::vector<ListenFunc>>::iterator EventEntryIt;
	typedef std::map<std::string, std::vector<ListenFunc>>::reference EventEntryRef;


	class EventManager: public Singleton<EventManager> {
		friend class Singleton<EventManager>;
		public:
		void addListener(std::string eventName, ListenFunc func);
		void signal(std::string eventName, ...) ;
		protected:
			EventManager(){}
			~EventManager(){}

		private:
			EventEntry _event_subs;
	};

}
#endif NOX_EVENT_MANAGER_H
