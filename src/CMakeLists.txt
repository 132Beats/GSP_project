# Copyright (c) 2019 by Tom Vierjahn

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(gsp1)

include_directories(./assets/*)
include_directories(./shader/*)


add_executable(main
		app.cpp
		app.hpp
		main.cpp
		window.cpp
		window.hpp
		geometry.cpp
		geometry.hpp 
		camera.cpp 
		camera.h 
		perlin.hpp 
		objLoader.cpp 
		objLoader.hpp)

# SDL2
find_package(SDL2 REQUIRED)
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	# Windows using thirdparty-folder via Moodle
	target_link_libraries(main
			PUBLIC SDL2::SDL2
			PUBLIC SDL2::SDL2main
			)
elseif ((CMAKE_SYSTEM_NAME MATCHES "Darwin"))
	# macOS using thirdparty libs via Homebrew
	# Ubuntu using thirdparty libs via apt
	target_include_directories(main
			PUBLIC ${SDL2_INCLUDE_DIRS}
			)
	target_link_libraries(main
			PUBLIC ${SDL2_LIBRARIES}
			)
else((CMAKE_SYSTEM_NAME MATCHES "Linux"))
	target_link_libraries(main
			PUBLIC
			SDL2
			)
endif()

# GLEW
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	# Ubuntu using thirdparty libs via apt
	find_package(GLEW REQUIRED)
	target_link_libraries(main
			PUBLIC GLEW::GLEW
			)
else()
	find_package(GLEW REQUIRED CONFIG)
	target_link_libraries(main
			PUBLIC GLEW::glew
			)
endif()

# OpenGL
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(OpenGL_GL_PREFERENCE GLVND)

	find_package(OpenGL REQUIRED)
	target_link_libraries(main
			PUBLIC OpenGL::GL
			)
else()
	find_package(OpenGL REQUIRED)
	target_link_libraries(main
			PUBLIC OpenGL::GL
			)
endif()

#glm
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	find_package(glm REQUIRED)
	target_link_libraries(main
			PUBLIC glm
			)
else()
	find_package(glm REQUIRED)
	target_link_libraries(main
			PUBLIC glm
			)
endif()