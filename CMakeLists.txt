cmake_minimum_required (VERSION 2.8)

# This is the name of the project
PROJECT( assignment3_task4 )
add_compile_options(-std=c++17 -Wall -Wextra -Wpedantic -g)

# This is for recursively searching for other directories.
# add_subdirectory (DirectoryName)

# sources to include
include_directories (.)
FIND_PACKAGE( OpenCV REQUIRED )

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

ADD_EXECUTABLE( assignment3_task4 main.cpp)
TARGET_LINK_LIBRARIES( assignment3_task4 ${OpenCV_LIBS} )

