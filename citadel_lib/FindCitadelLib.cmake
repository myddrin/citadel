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

get_filename_component(CITADELLIB_PREFIX ${CMAKE_CURRENT_LIST_FILE} PATH)

set(CITADELLIB_FOUND TRUE)
set(CITADELLIB_INCLUDE_DIRS ${CITADELLIB_PREFIX}/include)
set(CITADELLIB_DEFINITIONS)

set(CITADELLIB_NAME CitaldelLib)
set(CITADELLIB_LIBRARIES ${CITADELLIB_NAME})
