#include "log.h"

#include <SDL3/SDL_log.h>

namespace proto::log {

void log(Level level, const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	auto const log_level = [level]() {
		switch (level) {
		case Level::DEBUG: 	 return SDL_LOG_PRIORITY_DEBUG;
		case Level::INFO:	 return SDL_LOG_PRIORITY_INFO;
		case Level::WARN:  	 return SDL_LOG_PRIORITY_WARN;
		case Level::ERROR:	 return SDL_LOG_PRIORITY_ERROR;
		}
		return SDL_LOG_PRIORITY_INFO;
	}();
	SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, log_level, msg, args);
	va_end(args);
}

void info(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, msg, args);
	va_end(args);
}

void debug(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG, msg, args);
	va_end(args);
}

void warn(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, msg, args);
	va_end(args);
}

void error(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, msg, args);
	va_end(args);
}

} // namespace proto::log
