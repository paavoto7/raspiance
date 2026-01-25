cmake_minimum_required(VERSION 3.28)
project(Raspiance VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT LINUX)
    message(FATAL_ERROR "V4L2 (videodev2.h) is Linux-only. This project does not support macOS or Windows.")
endif()

find_path(V4L2_INCLUDE_DIR linux/videodev2.h)
if(NOT V4L2_INCLUDE_DIR)
    message(FATAL_ERROR "videodev2.h not found. Install libv4l-dev or equivalent.")
endif()

find_package(hueplusplus QUIET)
set(NEED_SUBMODULES NOT hueplusplus_FOUND)

find_package(GIT QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULES AND NEED_SUBMODULES)
        message(STATUS "Submodule updates")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
    endif()
endif()

option(USE_PERFORMANCE_FLAGS "Enable O3 and native optimisation" OFF)

# Perhaps move these to src so they don't apply later to libraries
if(USE_PERFORMANCE_FLAGS)
    message(STATUS "Performance mode enabled: Adding -O3 and -march=native")
    add_compile_options(-O3 -march=native)
endif()

add_subdirectory(src)

if(NOT hueplusplus_FOUND)
    message(STATUS "hueplusplus was not found, the submodule is used.")
    add_subdirectory("thirdparty/hueplusplus")
endif()

target_compile_options(raspiance PRIVATE -Wall -Wextra)
