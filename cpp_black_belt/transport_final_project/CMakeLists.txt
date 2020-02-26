cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
project(transport)

set(UTILSDIR ..)
set(SVGLIBDIR ../week_1/svg)

find_package(Protobuf REQUIRED)

file(GLOB SRCS 
    *.cpp 
    ${SVGLIBDIR}/svg.cpp 
    ${UTILSDIR}/profile_advanced.cpp
)

add_executable(transport)

add_subdirectory(./transport_catalog)

file(GLOB_RECURSE PROTO_FILES proto/*/*.proto)
protobuf_generate(
    LANGUAGE cpp
    TARGET transport
    IMPORT_DIRS proto
    APPEND_PATH proto
    PROTOC_OUT_DIR proto/generated
    PROTOS ${PROTO_FILES}
)
target_include_directories(
    transport
    PUBLIC
        proto/generated
)

target_sources(
    transport 
    PUBLIC 
        ${SRCS}
)
target_include_directories(
    transport
    PUBLIC
        ${Protobuf_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${UTILSDIR} 
        ${SVGLIBDIR}
)
target_compile_options(
    transport
    PUBLIC
        -g -O0 -Wall
)
if(SANITIZE)
target_compile_options(
    transport
        -fsanitize=address 
        -fsanitize=undefined 
        -fno-omit-frame-pointer 
        -fno-optimize-sibling-calls
)
endif()

if(DEBUG_MACROS)
target_compile_definitions(
    transport
    PUBLIC
        _GLIBCXX_DEBUG 
        _GLIBCXX_DEBUG_PEDANTIC
)
endif()

target_compile_definitions(
    transport
    PUBLIC
        DEBUG
)

target_link_libraries(transport ${Protobuf_LIBRARIES})