include(FetchContent)
message(STATUS "Resolving dependencies...")

message(STATUS "Resolving SDL3")
FetchContent_Declare(
	SDL
	GIT_REPOSITORY  https://github.com/libsdl-org/SDL.git
	GIT_TAG	        main
	GIT_SHALLOW     TRUE
	GIT_PROGRESS    TRUE
	FIND_PACKAGE_ARGS NAMES SDL3 CONFIG COMPONENTS
)

message(STATUS "Resolving GLM")
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG	     	33b0eb9fa336ffd8551024b1d2690e418014553b
	GIT_SHALLOW  	TRUE
	GIT_PROGRESS 	TRUE
	FIND_PACKAGE_ARGS
)

message(STATUS "Resolving nlohmann json")
FetchContent_Declare(
	json
	URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
	FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(SDL glm json)
message(STATUS "Finished resolving dependencies...")
