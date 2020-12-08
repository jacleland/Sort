# The latest source code can be downloaded from: [[URL]]
#
# Copyright (c) 2020, James A. Cleland <jcleland at jamescleland dot com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed Addin the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.10)

project(Sort)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src
)

find_package(CxxTest)
find_library(DL_LIBRARY libdl.so)

set(RADIXLIB_SOURCE_FILES
	src/radix.cpp
)

set(BUBBLELIB_SOURCE_FILES
	src/bubble.cpp
)

set(SORTLIB_SOURCE_FILES
	src/sortalgorithm.cpp
	src/sorter.cpp
)

set(MAIN_SOURCE_FILES
	src/main.cpp
)

add_library(RADIX SHARED ${RADIXLIB_SOURCE_FILES})
set_property(TARGET RADIX PROPERTY POSITION_INDEPENDENT_CODE 1)
set_property(TARGET RADIX PROPERTY CXX_STANDARD 17)
set_target_properties(RADIX PROPERTIES OUTPUT_NAME radix)
target_link_libraries(RADIX SORTLIB)

add_library(BUBBLE SHARED ${BUBBLELIB_SOURCE_FILES})
set_property(TARGET BUBBLE PROPERTY POSITION_INDEPENDENT_CODE 1)
set_property(TARGET BUBBLE PROPERTY CXX_STANDARD 17)
set_target_properties(BUBBLE PROPERTIES OUTPUT_NAME bubble)
target_link_libraries(BUBBLE SORTLIB)

add_library(SORTLIB SHARED ${SORTLIB_SOURCE_FILES})
set_property(TARGET SORTLIB PROPERTY POSITION_INDEPENDENT_CODE 1)
set_property(TARGET SORTLIB PROPERTY CXX_STANDARD 17)
target_link_libraries(SORTLIB ${DL_LIBRARY})
set_target_properties(SORTLIB PROPERTIES OUTPUT_NAME sortlib)

add_executable(MAIN ${MAIN_SOURCE_FILES})
set_property(TARGET MAIN PROPERTY POSITION_INDEPENDENT_CODE 1)
set_property(TARGET RADIX PROPERTY CXX_STANDARD 17)
set_target_properties(MAIN PROPERTIES OUTPUT_NAME isort)
target_link_libraries(MAIN ${DL_LIBRARY} SORTLIB)

install(
	TARGETS SORTLIB RADIX
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/
	DESTINATION include/Sort
	FILES_MATCHING PATTERN "sortalgorithm.h*"
)
