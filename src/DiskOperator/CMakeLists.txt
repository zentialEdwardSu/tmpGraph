include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/DiskOperator
)
aux_source_directory(. DIR_DiskUtils_SRCS)

add_library(DiskUtils ${DIR_DiskUtils_SRCS})