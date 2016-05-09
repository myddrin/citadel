cmake_minimum_required(VERSION 3.0)

find_package(CitadelLib REQUIRED)

message(STATUS "Build ${CITADELLIB_NAME}")

include_directories(${CITADELLIB_INCLUDE_DIRS})
add_definitions(${CITADELLIB_DEFINITIONS})

# use ${CMAKE_CURRENT_SOURCE_DIR}/ to have full path when running cpplint on VS
set(INCLUDES
	${CMAKE_CURRENT_SOURCE_DIR}/include/citadel/deck.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/citadel/building.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/citadel/player.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/citadel/game.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/citadel/character.hpp
)

set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/source/deck.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/building.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/player.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/game.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/game_factory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/character.cpp
)

add_library(${CITADELLIB_NAME} ${SOURCES} ${INCLUDES})

set(cpplint_cfg "${CMAKE_CURRENT_SOURCE_DIR}/CPPLINT.cfg")
if (EXISTS ${cpplint_cfg})
    message(STATUS "    add cpplint check")
    check_cpplint(${CITADELLIB_NAME} ${cpplint_cfg})
endif()