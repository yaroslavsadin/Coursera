cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
project(transport)

set(UTILSDIR ..)
set(SVGLIBDIR ../week_1/svg)

# Bringing in Protobuf libraries
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#Bring the headers into the project
include_directories(${UTILSDIR} ${SVGLIBDIR})

#Set preprocessor definitions
add_compile_definitions(DEBUG)

#Set compile and link flags
set(COMMON_FLAGS -g -O0 -Wall)

if(SANITIZE)
set(COMMON_FLAGS ${COMMON_FLAGS} -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls)
endif()

if(DEBUG_MACROS)
add_compile_definitions(_GLIBCXX_DEBUG _GLIBCXX_DEBUG_PEDANTIC)
endif()

add_compile_options(${COMMON_FLAGS})
add_link_options(${COMMON_FLAGS})

# Generate CXX from Proto
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS transport_catalog.proto database.proto router.proto)

#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES *.cpp ${SVGLIBDIR}/svg.cpp ../profile_advanced.cpp)
set(SOURCES ${PROTO_SRCS} ${PROTO_HDRS} ${SOURCES})

add_executable(transport ${SOURCES})
target_link_libraries(transport ${Protobuf_LIBRARIES})
