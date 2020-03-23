cmake_minimum_required(VERSION 3.10)

############### Running protoc ################
add_library(transport_catalog_proto OBJECT)
file(GLOB_RECURSE PROTO_FILES ./proto/*.proto)
protobuf_generate(
    LANGUAGE cpp
    TARGET transport_catalog_proto
    IMPORT_DIRS ./proto ../yellow_pages/proto
    APPEND_PATH ./proto
    PROTOC_OUT_DIR ./proto/generated
    PROTOS ${PROTO_FILES}
)
###############################################

############### Adding objects ################
file(GLOB SRCS *.cpp)

add_library(transport_catalog OBJECT ${SRCS} ${PROTO_HDRS})
###############################################

#### Make the library depend on protobuf generated headers
add_dependencies(transport_catalog_proto yellow_pages_proto)
add_dependencies(transport_catalog transport_catalog_proto)

# add_subdirectory(tests)