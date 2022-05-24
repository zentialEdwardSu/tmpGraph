cmake_minimum_required(VERSION 3.20)
project(tmpGraph)
set(CMAKE_C_STANDARD 11)

#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
#
#aux_source_directory(. Main_SRCS)
#
#add_subdirectory(src)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(
        include
)

add_executable(tmpGraph
    tmpGraph.c
)



