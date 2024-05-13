#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
#include <stdarg.h>
#include "locker.h"

class log {
public:
	static log* get_instance() {
		static log instance;
		return &instance;
	}
	void write_log(int level, const char* format, ...);

private:
	locker m_mutex;
};

#define LOG_DEBUG(format, ...) {log::get_instance()->write_log(0, format, ##__VA_ARGS__);}
#define LOG_INFO(format, ...) {log::get_instance()->write_log(1, format, ##__VA_ARGS__);}
#define LOG_WARN(format, ...) {log::get_instance()->write_log(2, format, ##__VA_ARGS__);}
#define LOG_ERROR(format, ...) {log::get_instance()->write_log(3, format, ##__VA_ARGS__);}

#endif