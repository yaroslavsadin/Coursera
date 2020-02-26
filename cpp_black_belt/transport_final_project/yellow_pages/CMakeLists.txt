cmake_minimum_required(VERSION 3.10)

file(GLOB SRCS 
    *.cpp
)

file(GLOB PROTO_FILES ./proto/*.proto)
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
        ./proto/generated
)

target_sources(
    transport 
    PUBLIC 
        ${SRCS}
)