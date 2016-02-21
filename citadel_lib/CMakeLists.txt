cmake_minimum_required(VERSION 3.0)

find_package(CitadelLib REQUIRED)

message(STATUS "Build ${CITADELLIB_NAME}")

include_directories(${CITADELLIB_INCLUDE_DIRS})
add_definitions(${CITADELLIB_DEFINITIONS})

set(INCLUDES
	include/citadel/deck.hpp
	include/citadel/building.hpp
	include/citadel/player.hpp
	include/citadel/game.hpp
	include/citadel/character.hpp
)

set(SOURCES
	source/deck.cpp
	source/building.cpp
	source/player.cpp
	source/game.cpp
	source/game_factory.cpp
	source/character.cpp
)

add_library(${CITADELLIB_NAME} ${SOURCES} ${INCLUDES})
