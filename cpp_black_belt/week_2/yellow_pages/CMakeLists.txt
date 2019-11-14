cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf REQUIRED)
find_path(TEST_RUNNER_PATH test_runner.h)

add_compile_options(-g -O0)
add_link_options(-g -O0)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${TEST_RUNNER_PATH})
include_directories(${Protobuf_INCLUDE_DIRS}/google/protobuf/util)

protobuf_generate_cpp(
    PROTO_SRCS
    PROTO_HDRS
    protos/address.proto
    protos/company.proto
    protos/name.proto
    protos/phone.proto
    protos/provider.proto
    protos/signal.proto
    protos/url.proto
    protos/working_time.proto
)

add_executable(yellow_pages ${PROTO_SRCS} ${PROTO_HDRS} ${TEST_RUNNER_PATH}/test_runner.cpp test.cpp merge.cpp)
target_link_libraries(yellow_pages ${Protobuf_LIBRARIES})
