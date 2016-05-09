cmake_minimum_required(VERSION 3.0)

project(citadel)

include (build/cpplint.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
	${CMAKE_CURRENT_SOURCE_DIR}/citadel_lib
)

add_subdirectory(citadel_lib)
add_subdirectory(citadel_cmd)
