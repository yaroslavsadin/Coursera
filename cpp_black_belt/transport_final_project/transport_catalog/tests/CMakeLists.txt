cmake_minimum_required(VERSION 3.10)

set(UTILSDIR ../../..)
set(SVGLIBDIR ../../../week_1/svg)

add_executable(transport_catalog_tests)
target_sources(
    transport_catalog_tests 
    PUBLIC 
        ${CMAKE_CURRENT_BINARY_DIR}/main.cpp
        ${UTILSDIR}/profile_advanced.cpp
        ${SVGLIBDIR}/svg.cpp
)
target_link_libraries(transport_catalog_tests 
    ${Protobuf_LIBRARIES}
    transport_catalog
    transport_catalog_proto
    json
)
target_include_directories(transport_catalog_tests
    PRIVATE
        ../proto/generated
)