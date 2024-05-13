#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>
#include "log.h"
#include "timestamp.h"

std::string toString() {
    time_t t = time(NULL);
    char buf[128] = {0};
    tm* tm_time = localtime(&t);
    snprintf(buf, sizeof(buf), "%4d-%02d-%02d",
             tm_time->tm_year + 1900,
             tm_time->tm_mon + 1,
             tm_time->tm_mday);
    return std::string(buf);
}

void log::write_log(int level, const char* format, ...) {
    char s[16] = {0};
    switch (level) {
    case 0:
        strcpy(s, "[Debug]:");
        break;
    case 1:
        strcpy(s, "[Info]:");
        break;
    case 2:
        strcpy(s, "[Warn]:");
        break;
    case 3:
        strcpy(s, "[Error]:");
        break;
    default:
        strcpy(s, "[Info]:");
        break;
    }
	m_mutex.lock();
    m_fp = fopen(toString().c_str(), "a+");
    if (m_fp == NULL) {
        perror("Error opening file");
        return;
    }
	char logBuffer[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(logBuffer, sizeof logBuffer, format, args);
	va_end(args);
    fprintf(m_fp,"%s %s %s\n",s,timestamp::now().toString().c_str(),logBuffer);
    fclose(m_fp);
	m_mutex.unlock();
}
