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


add_library(yellow_pages_proto OBJECT)
file(GLOB_RECURSE PROTO_FILES proto/*.proto)
protobuf_generate(
    LANGUAGE cpp
    TARGET yellow_pages_proto
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
file(GLOB PROTO_HDRS proto/generated/*.h)
target_sources(transport PUBLIC ${PROTO_HDRS})