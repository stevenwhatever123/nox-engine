#include <Utils/Utils.h>
#include <Utils/MemAllocator.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <cstdarg>

using namespace NoxEngineUtils;

using NoxEngine::ScratchMemAllocator;


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

FILETIME FileUtils::getLastWriteTime(const String& file) {
	WIN32_FILE_ATTRIBUTE_DATA file_attrib;
	i32 success = GetFileAttributesExA(file.c_str(), GetFileExInfoStandard, &file_attrib);
	if(success == 0) {
		LOG_DEBUG("Couldn't get latest file write time: %s", file.c_str());
	}
	assert(success == 1);
	return file_attrib.ftLastWriteTime;
}


void _print_cwd(const char *str, i32 line) {
	static char buf[240];
	GetCurrentDirectoryA(240, buf);
	Logger::debug("cwd: %s(%d) %s", str, line, buf);
}

