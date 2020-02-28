cmake_minimum_required(VERSION 3.10)

file(GLOB SRCS 
    *.cpp
)

target_sources(
    yellow_pages 
    PUBLIC 
        ${SRCS}
)