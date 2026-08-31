include (FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
	SDL
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG a96677bdf6b4acb84af4ec294e5f60a4e8cbbe03 #release-3.2.22
	GIT_PROGRESS TRUE
)

FetchContent_Declare(
	SDL_image 
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
	GIT_TAG 11154afb7855293159588b245b446a4ef09e574f #release-3.2.4
	GIT_PROGRESS TRUE
)

FetchContent_Declare(
	glm 
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG a532f5b1cf27d6a3c099437e6959cf7e398a0a67 #release-GLM 1.0.2
	GIT_PROGRESS TRUE
)

set(SDLIMAGE_DAV1D_ASM OFF)
set(SDLIMAGE_AVIF OFF)

FetchContent_MakeAvailable(SDL SDL_image glm)

FetchContent_Declare(
	json
	URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(SDL SDL_image GLM json)

add_library(
	glad STATIC 
	glad/src/gl.c
)

target_include_directories(glad PUBLIC glad/include)