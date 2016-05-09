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

find_package(CitadelLib REQUIRED)

message(STATUS "Build Citadel_cmd")

include_directories(${CITADELLIB_INCLUDE_DIRS})
add_definitions(${CITADELLIB_DEFINITIONS})

set(SOURCES
	citadel_cmd.cpp
)

add_executable(Citadel_cmd ${SOURCES})
target_link_libraries(Citadel_cmd ${CITADELLIB_LIBRARIES})
