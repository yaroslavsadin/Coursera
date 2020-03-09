cmake_minimum_required(VERSION 3.10)

###############################################
set(CMAKE_CXX_STANDARD 17)
project(transport)
###############################################

########## Setting dirs variables #############
set(UTILSDIR ..)
set(SVGLIBDIR ../week_1/svg)
###############################################

find_package(Protobuf REQUIRED)

########## Global project settings ############
include_directories(
    ${Protobuf_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${UTILSDIR} 
    ${SVGLIBDIR}
    ./transport_catalog
    ./transport_catalog/proto/generated
    ./yellow_pages
    ./yellow_pages/proto/generated
    ./json
)
add_compile_options(
    -g -O0 -Wall
)
if(SANITIZE)
add_compile_options(
        -fsanitize=address 
        -fsanitize=undefined 
        -fno-omit-frame-pointer 
        -fno-optimize-sibling-calls
)
endif()

if(DEBUG_MACROS)
add_compile_definitions(
    _GLIBCXX_DEBUG 
    _GLIBCXX_DEBUG_PEDANTIC
)
endif()

add_compile_definitions(
    DEBUG
)

get_property(CXX____FLAGS DIRECTORY PROPERTY COMPILE_OPTIONS)
add_link_options(
    ${CXX____FLAGS}
)

file(GLOB SRCS 
    *.cpp 
    ${SVGLIBDIR}/svg.cpp 
    ${UTILSDIR}/profile_advanced.cpp
)

############### Main executable ###############
if(DEFINED BUILD_MAIN)
    add_executable(transport)
    target_sources(
        transport 
        PUBLIC 
            ${SRCS}
    )
    target_link_libraries(transport 
        ${Protobuf_LIBRARIES} 
        transport_catalog
        transport_catalog_proto 
        yellow_pages
        yellow_pages_proto
        json
    )
endif()
###############################################

add_subdirectory(./json)
add_subdirectory(./transport_catalog)
add_subdirectory(./yellow_pages)

enable_testing()

add_test(NAME transport_catalog_tests COMMAND transport_catalog_tests)
add_test(NAME yellow_pages_tests COMMAND yellow_pages_tests)
add_test(NAME transport COMMAND transport)