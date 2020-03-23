cmake_minimum_required(VERSION 3.10)

############### Running protoc ################
add_library(yellow_pages_proto OBJECT)
file(GLOB_RECURSE PROTO_FILES ./proto/*.proto)
protobuf_generate(
    LANGUAGE cpp
    TARGET yellow_pages_proto
    IMPORT_DIRS ./proto
    APPEND_PATH ./proto
    PROTOC_OUT_DIR ./proto/generated
    PROTOS ${PROTO_FILES}
)
###############################################

############### Adding objects ################
file(GLOB SRCS *.cpp)

add_library(yellow_pages OBJECT ${SRCS} ${PROTO_HDRS})
###############################################

#### Make the library depend on protobuf generated headers
add_dependencies(yellow_pages yellow_pages_proto)

# add_subdirectory(tests)