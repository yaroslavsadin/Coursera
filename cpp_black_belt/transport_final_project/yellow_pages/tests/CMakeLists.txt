cmake_minimum_required(VERSION 3.10)

set(UTILSDIR ../../..)

add_executable(yellow_pages_tests)
target_sources(
    yellow_pages_tests 
    PUBLIC 
        ${CMAKE_CURRENT_BINARY_DIR}/main.cpp
        ${UTILSDIR}/profile_advanced.cpp
)
target_link_libraries(yellow_pages_tests 
    ${Protobuf_LIBRARIES}
    yellow_pages
    yellow_pages_proto
)
target_include_directories(yellow_pages_tests
    PRIVATE
        ..
        ../proto/generated
)