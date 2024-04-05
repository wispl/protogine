#pragma once

#include <string>

namespace filesystem {

std::string read_file(const char* path);
std::string parent_dir(const char* path);
}
