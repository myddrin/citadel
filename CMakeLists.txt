#
# Copyright (c) 2016 Thomas Richard
#
# Following MIT license (see copying.txt)
#
# The software is provided "as is", without warranty of any kind, express or
# implied, including but not limited to the warranties of merchantability,
# fitness for a particular purpose and noninfringement.
#
cmake_minimum_required(VERSION 3.0)

project(citadel)

include (build/cpplint.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
	${CMAKE_CURRENT_SOURCE_DIR}/citadel_lib
)

add_subdirectory(citadel_lib)
add_subdirectory(citadel_cmd)
