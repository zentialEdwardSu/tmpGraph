cmake_minimum_required(VERSION 3.20)
project(tmpGraph)
set(CMAKE_C_STANDARD 11)

add_subdirectory(
        src/DiskOperator
)

add_subdirectory(
        src/GraphUtils
)


set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(
        include
)

add_executable(tmpGraph
    tmpGraph.c
)

target_link_libraries(tmpGraph DiskUtils)
target_link_libraries(tmpGraph GraphUtils)



