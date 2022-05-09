#pragma once

#define WIN32_LEAN_AND_MEAN
#include "NoxEngineWindows.h"

#include <Core/Types.h>

namespace NoxEngineUtils {

	class Logger {
		 enum {
			 MAX_TEMP_BUFFER = 1024
		 };

		public:
			static void debug(String fmt_str, ...);

		private:
			Logger(){};
			~Logger();
	};

	class FileUtils {
		public:
			static FILETIME getLastWriteTime(const String& file_path);
	};

	class MemUtils {
		public:
			inline static i32 Bytes (i32 size_bytes) { return size_bytes; }
			inline static i32 KBytes(i32 size_bytes) { return Bytes (size_bytes)*1024; }
			inline static i32 MBytes(i32 size_bytes) { return KBytes(size_bytes)*1024; }
			inline static i64 GBytes(i32 size_bytes) { return MBytes(size_bytes)*1024; }
	};

}


#define LOG_DEBUG(str, ...) NoxEngineUtils::Logger::debug( "%s(%d) " str, __FILE__, __LINE__, __VA_ARGS__ )
#define print_cwd() _print_cwd(__FILE__, __LINE__)
void _print_cwd(const char *str, i32 line);

#define HEX_TO_FLOAT4(hex) {  ((hex >> 24) & 0xFF)/255.f, ((hex >> 16) & 0xFF)/255.0f, ((hex>>8) & 0xFF)/255.0f, (hex & 0xFF)/255.0f }
#define FLOAT4_TO_HEX(float4) \
							  ((i32)(float4[0]*255.0f) << 24)\
							| ((i32)(float4[1]*255.0f) << 16)\
							| ((i32)(float4[2]*255.0f) <<  8)\
							| ((i32)(float4[3]*255.0f) <<  0)\
