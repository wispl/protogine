#include "filesystem.h"

#include "../services/log.h"

#include <SDL3/SDL.h>

#include <filesystem>
#include <fstream>
#include <string>

namespace filesystem {

std::string read_file(const char* path)
{
	std::ifstream file;
	file.open(path, std::ios::in);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file: %s");
	}
	return std::string(std::istreambuf_iterator<char>(file),
		    	   std::istreambuf_iterator<char>());

}

std::string parent_dir(const char* path)
{
	std::filesystem::path p{path};
	return p.parent_path();
}

} // end namespace filesystem
