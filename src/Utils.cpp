#include <Utils.h>
#include <Types.h>
#include <cstdarg>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>

#include <MemAllocator.h>
using NoxEngine::ScratchMemAllocator;

namespace NoxEngineUtils {

	void Logger::debug(std::string fmt_str, ...) {
		va_list arg_list;
		va_start(arg_list, fmt_str);

		char* temp_buf = reinterpret_cast<char*>(ScratchMemAllocator::Instance()->allocate(MAX_TEMP_BUFFER));

		vsprintf_s(temp_buf, MAX_TEMP_BUFFER, fmt_str.c_str(), arg_list);
		va_end(arg_list);

		static i64 message_id = 0;
		message_id++;
		const std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		struct tm tm_buffer = {0};
		localtime_s(&tm_buffer, &time);
		std::cout << "[" <<  std::put_time(&tm_buffer, "%I:%M:%S") << "][DEBUG][" << message_id << "] " << temp_buf << std::endl;
		fflush(stdout);

	}

}
