cmake_minimum_required(VERSION 3.13)

option(ENABLE_CONSOLE "Enable console window in the application" ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

include(CreateCmpCmdsLink)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

project(AntLife2)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES 
	src/main.cpp
  src/world.cpp
  src/ant.cpp

	# EasyLogingpp
	deps/easyloggingpp/easylogging++.h
	deps/easyloggingpp/easylogging++.cc
)
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)

add_executable(antLife ${SOURCE_FILES})

include_directories( src/ deps/glm/ deps/sfml/include/ deps/glm/glm/gtc/ deps/easyloggingpp/ deps/nano-signal-slot/)
add_definitions(-DELPP_THREAD_SAFE)
set(SFML_STATIC_LIBRARIES TRUE)
add_definitions(-DSFML_STATIC)


#add_dependencies(antLife copy_resources)
add_dependencies(antLife create_compile_commands_link)
target_link_directories(antLife PRIVATE ./deps/sfml/lib/)
target_link_libraries(antLife
  sfml-graphics-s
  sfml-window-s
  sfml-system-s
  opengl32
  winmm
  gdi32
  shell32
  user32
  advapi32
  ole32
  oleaut32
  uuid
  comdlg32
)
