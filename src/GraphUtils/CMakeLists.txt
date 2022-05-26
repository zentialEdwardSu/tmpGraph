include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/GraphUtils
)
aux_source_directory(. DIR_GraphUtils_SRCS)

add_library(GraphUtils ${DIR_GraphUtils_SRCS})