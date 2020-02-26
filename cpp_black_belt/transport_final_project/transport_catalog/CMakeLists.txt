cmake_minimum_required(VERSION 3.10)

file(GLOB SRCS 
    *.cpp
)

target_sources(
    transport 
    PUBLIC 
        ${SRCS}
)
target_include_directories(
    transport
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
)