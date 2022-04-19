#pragma once

#include <Types.h>
#include <string>


namespace NoxEngineUtils {

	class Logger {
		enum {
			MAX_TEMP_BUFFER = 1024
		};

		public:
			static void debug(std::string fmt_str, ...);
		private:
			Logger(){};
	};


	class MemUtils {
		public:
			inline static i32 Bytes (i32 size_bytes) { return size_bytes; }
			inline static i32 KBytes(i32 size_bytes) { return Bytes (size_bytes)*1024; }
			inline static i32 MBytes(i32 size_bytes) { return KBytes(size_bytes)*1024; }
			inline static i64 GBytes(i32 size_bytes) { return MBytes(size_bytes)*1024; }
	};

}
