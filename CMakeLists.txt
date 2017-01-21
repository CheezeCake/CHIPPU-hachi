cmake_minimum_required(VERSION 3.3)

project(CHIPPU-hachi)

add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=c++14>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wall>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-g>)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2_MIXER REQUIRED SDL2_mixer)

set(CHIP8_DIR "CHIP-8/")
set(SDL2_DIR "SDL2/")

set(SRCS
	${CHIP8_DIR}VM.cpp
	${CHIP8_DIR}Memory.cpp
	${CHIP8_DIR}Display.cpp
	${CHIP8_DIR}Timer.cpp
	${SDL2_DIR}SDL2.cpp
	${SDL2_DIR}Main.cpp
)

set(EXEC_NAME chippu-hachi)

include_directories(".")
add_executable(${EXEC_NAME} ${SRCS})

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${EXEC_NAME} ${SDL2_LIBRARIES})

include_directories(${SDL2_MIXER_INCLUDE_DIRS})
target_link_libraries(${EXEC_NAME} ${SDL2_MIXER_LIBRARIES})
