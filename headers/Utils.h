#ifndef NOX_UTILS_H
#define NOX_UTILS_H

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
			static char text_temp_buf[MAX_TEMP_BUFFER];
	};

}



#endif // NOX_UTILS_H

