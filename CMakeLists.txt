cmake_minimum_required(VERSION 3.3)

project(CHIPPU-hachi)

option(SDL2_BACKEND "Build with the SDL2 backend" OFF)
option(NCURSES_BACKEND "Build with the ncurses backend" OFF)

add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=c++14>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wall>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-g>)

set(EXEC_NAME chippu-hachi)
set(CHIP8_DIR "CHIP-8/")

set(SRC
	${CHIP8_DIR}VM.cpp
	${CHIP8_DIR}Memory.cpp
	${CHIP8_DIR}Display.cpp
	${CHIP8_DIR}Timer.cpp
)


include(FindPkgConfig)

if(SDL2_BACKEND)
	pkg_search_module(SDL2 REQUIRED sdl2)
	pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer)

	set(BACKEND_DIR "SDL2/")
	list(APPEND SRC ${BACKEND_DIR}SDL2.cpp)
	file(COPY ${BACKEND_DIR}beep.mp3 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
elseif(NCURSES_BACKEND)
	pkg_search_module(NCURSES REQUIRED ncurses)

	set(BACKEND_DIR "ncurses/")
	list(APPEND SRC ${BACKEND_DIR}Ncurses.cpp)
	list(APPEND SRC ${BACKEND_DIR}Clock.cpp)
else()
	message(FATAL_ERROR "Please choose a backend to build.")
endif()

# add Main.cpp
list(APPEND SRC ${BACKEND_DIR}Main.cpp)

include_directories(".")
add_executable(${EXEC_NAME} ${SRC})

# SDL2
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${EXEC_NAME} ${SDL2_LIBRARIES})
# SDL2_mixer
include_directories(${SDL2_MIXER_INCLUDE_DIRS})
target_link_libraries(${EXEC_NAME} ${SDL2_MIXER_LIBRARIES})

# ncurses
include_directories(${NCURSES_INCLUDE_DIRS})
target_link_libraries(${EXEC_NAME} ${NCURSES_LIBRARIES})
