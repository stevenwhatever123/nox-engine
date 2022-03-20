#ifndef NOX_UTILS_H
#define NOX_UTILS_H

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
			inline static i64 Bytes(i64 size_bytes)  { return size_bytes; }
			inline static i64 KBytes(i64 size_bytes) { return size_bytes; }
			inline static i64 MBytes(i64 size_bytes) { return KBytes(size_bytes)*1024; }
			inline static i64 GBytes(i64 size_bytes) { return MBytes(size_bytes)*1024; }
	};

}



#endif // NOX_UTILS_H

