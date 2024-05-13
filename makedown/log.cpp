#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>
#include "log.h"
#include "timestamp.h"

void log::write_log(int level, const char* format, ...) {
	m_mutex.lock();
	switch (level) {
	case 0:
		std::cout << "[Debug]:";
		break;
	case 1:
		std::cout << "[Info]:";
		break;
	case 2:
		std::cout << "[Warn]:";
		break;
	case 3:
		std::cout << "[Erro]:";
		break;
	default:
		std::cout << "[Info]:";
		break;
	}
	char logBuffer[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(logBuffer, sizeof logBuffer, format, args);
	va_end(args);
	std::cout << timestamp::now().toString() << " : " << logBuffer << std::endl;
	m_mutex.unlock();
}
