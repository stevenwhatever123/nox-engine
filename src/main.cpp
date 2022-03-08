#define GLEW_STATIC
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <vector>
#include <map>

#include <GL/glew.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cstdarg>
#include <fmod.hpp>
#include <fmod_common.h>

enum {
	MAX_TEMP_BUFFER = 1024
};

char text_temp_buf[MAX_TEMP_BUFFER];

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float  f32;
typedef double f64;


template <typename Type>
class Singleton {
	public:
		static Type* Instance() {
			static Type* inst = new Type;
			return inst;
		}

	protected:
		Singleton(){};
		virtual ~Singleton() {};

	private:
		Singleton(const Singleton &other);

};


void log_debug(std::string fmt_str, ...) {

	va_list arg_list;
	va_start(arg_list, fmt_str);
	vsprintf(text_temp_buf, fmt_str.c_str(), arg_list);
	va_end(arg_list);

	static i64 message_id = 0;
	message_id++;
	const std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::cout << "[" <<  std::put_time(std::localtime(&time), "%I:%M:%S") << "][DEBUG][" << message_id << "] " << text_temp_buf << std::endl;


}

typedef void (*ListenFunc)(void);
typedef std::map<std::string, std::vector<ListenFunc>> EventEntry;
typedef std::map<std::string, std::vector<ListenFunc>>::iterator EventEntryIt;
typedef std::map<std::string, std::vector<ListenFunc>>::reference EventEntryRef;

class EventManager: public Singleton<EventManager> {

	friend class Singleton<EventManager>;

	public:
		void addListener(std::string eventName, ListenFunc func) {

			if(_event_subs.contains(eventName)) {
				EventEntryIt entry = _event_subs.find(eventName);
				entry->second.push_back(func);
			} else {
				_event_subs.insert(std::pair(eventName, std::vector<ListenFunc>{func}));
			}
		}

		void signal(std::string eventName) {
			if(_event_subs.contains(eventName)) {
				auto it = _event_subs[eventName];
				for(i32 i = 0; i < it.size(); i++) {
					log_debug("Hello ptr: %p", it[i]);
					it[i]();
				}
				log_debug("hello world");
			} else {

			}
		}

	protected:
		EventManager(){}
		~EventManager(){}

	private:
		EventEntry _event_subs;
};

int main(int argc, char** argv) {
	FMOD::System* system;
	FMOD::Sound* sound1, * sound2, * sound3;
	FMOD::Channel* channel = 0;
	FMOD_RESULT       result;
	
	result = FMOD::System_Create(&system);
	


	return 0;

	EventManager *event_manager = EventManager::Instance();

	event_manager->addListener("meow", []() { log_debug("Hello World From event Meow"); });
	event_manager->addListener("meow", []() { log_debug("Second Hello world"); });

	event_manager->addListener("onDraw", []() { log_debug("func 2"); });
	event_manager->addListener("onDraw", []() { log_debug("func 1"); });


	event_manager->signal("onDraw");

	event_manager->signal("meow");

	return 0;
}
