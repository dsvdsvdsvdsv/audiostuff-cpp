project(CommonIncludes)

set(SOURCES Compressor.cpp)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE .
)
