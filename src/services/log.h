#pragma once

namespace proto::log {

enum class Level { INFO, DEBUG, WARN, ERROR };

void log(Level level, const char* msg, ...);
void info(const char* msg, ...);
void debug(const char* msg, ...);
void warn(const char* msg, ...);
void error(const char* msg, ...);

}
